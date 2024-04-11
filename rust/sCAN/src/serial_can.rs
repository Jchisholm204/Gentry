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
        let mut buffer : [CANmsg; BUFFER_LEN];
        let mut read_index : usize = 0;
        let mut write_index: usize = 0;
        let mut temp_msg : CANmsg = CANmsg::default();

        loop{
            // Begin Receiver
            let mut rx_buffer: [u8; 64] = [0; 64];
            match port.read(&mut rx_buffer){
                Ok(mut bytes) => {
                    while bytes > 0 {
                        match state {
                            State::ID => {}
                            State::LEN => {}
                            State::Data => {}
                            State::CRC => {}
                        }
                        bytes -= 1;
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
