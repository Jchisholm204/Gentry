use serialport;
use std::time::Duration;
use std::{io, thread};
use std::io::Write;
use std::sync::mpsc::Sender;
use std::sync::mpsc::Receiver;


pub struct Serial {
    pub tx: Sender<u8>,
    pub rx: Receiver<u8>,
}

impl Serial {
    pub fn new(p_name: &String, baud: u32) -> Self {
        let mut serial_port = serialport::new(p_name, baud).open().expect("Failed to open port");
        let _ = serial_port.set_timeout(Duration::from_millis(20));
        let (stx, srx) = std::sync::mpsc::channel::<u8>();
        let (rtx, rrx) = std::sync::mpsc::channel::<u8>();

        thread::spawn(move||{
            loop{
                if let Ok(data) = srx.try_recv(){
                    serial_port.write_all(&[data]).expect("Write Error");
                }
                let mut rx_buffer: [u8; 1] = [0; 1];
                match serial_port.read(&mut rx_buffer){
                    Ok(bytes) => {
                        if bytes != 0 {
                            let _ = rtx.send(rx_buffer[0]);
                        }
                    }
                    Err(ref e) if e.kind() == io::ErrorKind::TimedOut => {
                        thread::sleep(Duration::from_millis(10));
                    },
                    Err(e) => eprintln!("{:?}", e),
                }
            }
        });

        return Serial {
            rx:   rrx,
            tx:   stx,
        }
    }

}


