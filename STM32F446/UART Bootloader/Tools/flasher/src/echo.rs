use serialport::SerialPort;
use std::io;

pub fn echo_test(port : &mut Box<dyn SerialPort>, msg : &str) {
    //println!("Attempting Transmit");
    port.write_all(&msg.as_bytes()).expect("Transmit Failed");
    //println!("Completed Transmit");
    //println!("Attempting Read");
    let mut rx_buffer: [u8; 1] = [0; 1];
    match port.read(&mut rx_buffer){
        Ok(bytes) => {
            if bytes != 0 {
                print!("{}", rx_buffer[0] as char);
            }
        }
        Err(ref e) if e.kind() == io::ErrorKind::TimedOut => (),
        Err(e) => eprintln!("{:?}", e),
    }
    //println!("Sleeping");
    //thread::sleep(Duration::from_millis(200));
}

