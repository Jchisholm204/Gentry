use serialport;
use std::time::Duration;
use std::io;
use std::thread;
use std::sync::mpsc;

const BUFFER_LEN : usize = 64;

#[derive(Clone, Copy)]
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
            loop{
                Self::rx_tick(&mut serial_port, &mut rtx);
                Self::tx_tick(&mut serial_port, &mut trx);
            }
        });

        SerialCAN {
            receiver: rrx,
            transmitter : ttx,
        }
    }

    fn rx_tick(port : & mut Box<dyn serialport::SerialPort>, writer: &mut mpsc::Sender<CANmsg>){
        let mut rx_buffer: [u8; 64] = [0; 64];
        match port.read(&mut rx_buffer){
            Ok(bytes) => {
                if bytes != 0 {
                }
            },
            Err(_) => {}
        }
        Self::crc_check(&CANmsg::default());
    }

    fn tx_tick(port : & mut Box<dyn serialport::SerialPort>, reader: &mut mpsc::Receiver<CANmsg>){}

    fn crc_check(msg: &CANmsg) -> bool {
        return false;
    }
}
