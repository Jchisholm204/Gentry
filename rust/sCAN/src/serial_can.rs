use serialport;
use serde::Serialize;
use bincode;
use std::time::Duration;
use std::thread;
use std::sync::mpsc;

const BUFFER_LEN : usize = 64;
const RESEND_ID  : u32   = 404;

#[derive(Clone, Copy, Serialize)]
pub struct CANmsg{
    pub id : u32,
    pub data : [u8; 8],
    pub len : u8,
    pub crc : u8,
}


impl CANmsg {
    pub fn default() -> Self {
        let mut msg = CANmsg {
            id : 0,
            data : [0; 8],
            len: 0,
            crc: 0,
        };
        msg.gen_crc();
        return msg;
    }
    pub fn resend(rtx_id: u32) -> Self {
        let mut msg = CANmsg {
            id: RESEND_ID,
            len: 4,
            data: [(rtx_id >> 24) as u8, (rtx_id >> 16) as u8, (rtx_id >> 8) as u8, rtx_id as u8, 0, 0, 0, 0],
            crc: 0,
        };
        msg.gen_crc();
        return msg;
    }
    pub fn is_resend(&self) -> bool {
        return self.id == RESEND_ID;
    }

    pub fn ack() -> Self {
        let mut msg = CANmsg {
            id: 80,
            len: 0,
            data: [0; 8],
            crc: 0,
        };
        msg.gen_crc();
        return msg;
    }

    pub fn is_ack(&self) -> bool {
        if self.id != 80 { return false; }
        if self.len != 0 { return false; }
        return true;
    }

    pub fn check_crc(&self) -> bool {
        let mut cpy = self.clone();
        cpy.gen_crc();
        if cpy.crc == self.crc { return true }
        else { return false; }
    }

    pub fn gen_crc(&mut self) { unimplemented!() }
}

enum State {
    ID, LEN, Data, CRC,
}

pub struct SerialCAN{
    pub receiver    : mpsc::Receiver<CANmsg>,
    pub transmitter : mpsc::Sender<CANmsg>,

}

impl SerialCAN {
    pub fn new(port_name: String, baud : u32) -> Self{
        let (ttx, trx) = mpsc::channel();
        let (rtx, rrx) = mpsc::channel();
        
        let serial_port = serialport::new(port_name.as_str(), baud).open().expect("Failed to open Serial Port");
        thread::spawn(move ||{
            let mut serial_port = serial_port;
            let _ = serial_port.set_timeout(Duration::from_millis(1));
            let mut trx = trx;
            let mut rtx = rtx;
            Self::tick(&mut serial_port, &mut rtx, &mut trx);
        });

        SerialCAN {
            receiver: rrx,
            transmitter : ttx,
        }
    }

    fn tick(port : & mut Box<dyn serialport::SerialPort>, writer: &mut mpsc::Sender<CANmsg>, reader: &mut mpsc::Receiver<CANmsg>){
        let mut state : State = State::ID;
        let mut temp_msg : CANmsg = CANmsg::default();
        let mut last_msg : CANmsg = CANmsg::default();
        let mut byte_count : usize = 0;

        loop{
            // Begin Receiver
            let mut byte_buffer: [u8; BUFFER_LEN] = [0; BUFFER_LEN];
            match port.read(&mut byte_buffer){
                Ok(bytes) => {
                    for i in 0..bytes {
                        match state {
                            State::ID => {
                                temp_msg.id |= ((byte_buffer[i] as u32) << (8*byte_count)) as u32;
                                byte_count += 1;
                                if byte_count >= 4 {
                                    byte_count = 0;
                                    state = State::LEN;
                                    break;
                                }
                            }
                            State::LEN => {
                                temp_msg.len = byte_buffer[i];
                                state = State::Data;
                                break;
                            }
                            State::Data => {
                                temp_msg.data[byte_count] = byte_buffer[i];
                                byte_count += 1;
                                if byte_count as u8 >= temp_msg.len {
                                    for j in byte_count..7 {
                                        temp_msg.data[j] = 0;
                                    }
                                    byte_count = 0;
                                    state = State::CRC;
                                    break;
                                }
                            }
                            State::CRC => {
                                temp_msg.crc = byte_buffer[i];
                                if temp_msg.check_crc() == false {
                                    let resend = CANmsg::resend(temp_msg.id);
                                    let bytes  = bincode::serialize(&resend).unwrap();
                                    let _ = port.write_all(&bytes);
                                    break;
                                }
                                if temp_msg.is_ack() {
                                    break;
                                }
                                if temp_msg.is_resend() {
                                    let bytes  = bincode::serialize(&last_msg).unwrap();
                                    let _ = port.write_all(&bytes);
                                    break;
                                }
                                let _ = writer.send(temp_msg);
                                temp_msg = CANmsg::default();
                                let ack = CANmsg::ack();
                                let bytes  = bincode::serialize(&ack).unwrap();
                                let _ = port.write_all(&bytes);
                                break;
                            }
                        }
                    }
                },
                Err(_) => {}
            }
            // End Receiver
            match reader.try_recv() {
                Ok(mut msg) => {
                    if msg.check_crc() == false {
                        msg.gen_crc();
                    }
                    let bytes = bincode::serialize(&msg).unwrap();
                    let _ = port.write_all(&bytes);
                    }
                Err(_) => {}
            }
        }
    }

}
