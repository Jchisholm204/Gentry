#![allow(unused)]
use serialport;

const BUFFER_LEN : usize = 64;

#[derive(Clone, Copy)]
pub struct CANmsg{
    pub id : u32,
    pub data : [u8; 8],
    pub len : u8,
}

impl CANmsg {
    pub fn default() -> CANmsg {
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
    serial_port : Box<dyn serialport::SerialPort>,
    state : State,
    buffer: [CANmsg; BUFFER_LEN],
}

impl SerialCAN {
    pub fn new(port_name: String, baud : u32) -> Self{
        SerialCAN{
            serial_port : serialport::new(port_name.as_str(), baud).open().expect("Failed to open Serial Port"),
            state : State::ID,
            buffer : [CANmsg::default(); BUFFER_LEN],
        }
    }
}
