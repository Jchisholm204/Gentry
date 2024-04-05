use serialport;
use std::time::Duration;
use std::{io, thread};

fn main() {

    let ports = serialport::available_ports().expect("No Ports Found!");
    for p in ports {
        println!("{}", p.port_name)
    }

    let port_name = &serialport::available_ports().expect("No Ports Found!")[1].port_name;
    println!("Connecting to port: {}", port_name);
    let mut port = serialport::new(port_name, 9600).open().expect("Failed to open port");


    let msg = "Hello\n";
    loop{
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
        thread::sleep(Duration::from_millis(200));
    }


}
