use serialport;
use serde::Serialize;
use bincode;
use std::time::Duration;
use std::slice;
use std::thread;
use std::sync::mpsc;

const BUFFER_LEN : usize = 64;

#[derive(Clone, Copy, Serialize)]
pub struct CANmsg{
    pub id : u32,
    pub data : [u8; 8],
    pub len : u8,
}


impl CANmsg {
    pub fn default() -> Self {
        CANmsg {
            id : 0,
            data : [0; 8],
            len: 0,
        }
    }
}

enum State {
    ID, LEN, Data, CRC,
}

pub struct SerialCAN{
    receiver    : mpsc::Receiver<CANmsg>,
    transmitter : mpsc::Sender<CANmsg>,

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
        let mut internal : usize = 0;

        loop{
            // Begin Receiver
            let mut byte_buffer: [u8; 64] = [0; 64];
            match port.read(&mut byte_buffer){
                Ok(bytes) => {
                    for i in 0..bytes {
                        match state {
                            State::ID => {
                                temp_msg.id |= (byte_buffer[i] as u32) << 8*internal;
                                internal += 1;
                                if internal >= 4 {
                                    internal = 0;
                                    state = State::LEN;
                                }
                            }
                            State::LEN => {}
                            State::Data => {}
                            State::CRC => {
                                if Self::crc_check(&temp_msg) {
                                    let _ = writer.send(temp_msg);
                                }
                            }
                        }
                    }
                },
                Err(_) => {}
            }
            // End Receiver
            match reader.try_recv() {
                Ok(msg) => {
                    let bytes = bincode::serialize(&msg).unwrap();
                    let _ = port.write_all(&bytes);
                    }
                Err(_) => {}
            }
        }
    }

    fn crc_check(msg: &CANmsg) -> bool {
        return false;
    }
}
