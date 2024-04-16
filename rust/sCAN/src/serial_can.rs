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
    pub len : u8,
    pub data : [u8; 8],
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
    pub fn resend(_rtx_id: u32) -> Self {
        let mut msg = CANmsg {
            id: RESEND_ID,
            len: 4,
            data: [0;8],//[(rtx_id >> 24) as u8, (rtx_id >> 16) as u8, (rtx_id >> 8) as u8, rtx_id as u8, 0, 0, 0, 0],
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
        if self.crc8() == self.crc { return true }
        else { return false; }
    }

    pub fn gen_crc(&mut self) {
        self.crc = self.crc8();
    }

    fn crc8(&self) -> u8 {
        let mut crc: u8 = 0;
        let bytes = self.serialize();
        for i in 0..12 as usize {
            crc ^= bytes[i];
            for _ in 0..8 {
                if crc & 0x80 != 0 {
                    crc = (crc << 1) ^ 0x07;
                }
                else {
                    crc = crc << 1;
                }
            }
        }
        return crc;
    }

    pub fn serialize(&self) -> Vec<u8> {
        return bincode::serialize(self).unwrap();
    }

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
                                }
                            }
                            State::LEN => {
                                //println!("RXID: {}", temp_msg.id);
                                temp_msg.len = byte_buffer[i];
                                if temp_msg.len > 8 { temp_msg.len = 8; }
                                state = State::Data;
                                byte_count = 0;
                            }
                            State::Data => {
                                temp_msg.data[byte_count] = byte_buffer[i];
                                byte_count += 1;
                                if byte_count >= 8 {
                                    byte_count = 0;
                                    state = State::CRC;
                                }
                            }
                            State::CRC => {
                                temp_msg.crc = byte_buffer[i];
                                if temp_msg.check_crc() == false {
                                    let _ = port.write_all(&CANmsg::ack().serialize());
                                    temp_msg = CANmsg::default();
                                }
                                else if temp_msg.is_ack() {
                                    temp_msg = CANmsg::default();
                                    //println!("ACK");
                                }                                
                                else if temp_msg.is_resend() {
                                    let _ = port.write_all(&last_msg.serialize());
                                    temp_msg = CANmsg::default();
                                }
                                else {
                                    //println!("ID: {}", temp_msg.id);
                                    let _ = writer.send(temp_msg);
                                    temp_msg = CANmsg::default();
                                    let _ = port.write_all(&CANmsg::ack().serialize());
                                }
                                state = State::ID;
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
                    last_msg = msg;
                    let bytes = bincode::serialize(&msg).unwrap();
                    let _ = port.write_all(&bytes);
                    }
                Err(_) => {}
            }
            thread::sleep(Duration::from_millis(10));
        }
    }

}
