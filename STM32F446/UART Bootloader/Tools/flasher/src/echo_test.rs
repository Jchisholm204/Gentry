use serialport::SerialPort;
use serialport;
use std::time::Duration;
use std::{io, thread};
use std::io::Write;

pub fn echo_test(port : &mut Box<dyn SerialPort>) {
    let (tx, rx) = std::sync::mpsc::channel();

    let _rx_handle = thread::spawn(move ||{
    loop {
        let mut input = String::new();
        match io::stdin().read_line(& mut input){
            Ok(_n) => {
                tx.send(input).expect("Failed to send Data");
            },
            Err(_) => thread::sleep(Duration::from_millis(100)),
        }
        thread::sleep(Duration::from_millis(100));
    }
    });

    loop {
        //println!("Hey");
        if let Ok(data) = rx.try_recv() {
            port.write_all(data.as_bytes()).expect("Write Error");
        }
        let mut rx_buffer: [u8; 1] = [0; 1];
        match port.read(&mut rx_buffer){
            Ok(bytes) => {
                if bytes != 0 {
                    print!("{}", rx_buffer[0] as char);
                }
                else{
                    println!("No Bytes");
                }
            }
            Err(ref e) if e.kind() == io::ErrorKind::TimedOut => {
                //println!("TimedOut");
                thread::sleep(Duration::from_millis(10));
            },
            Err(e) => eprintln!("{:?}", e),
        }
    }
}

