pub mod serial_can;
use std::time::Duration;
use std::thread;
use std::io;

use serial_can::CANmsg;

pub fn serial_main(){
    println!("Available Serial Ports");
    let ports = serialport::available_ports().expect("No Serial Ports Found");
    for (i, port) in ports.iter().enumerate() {
        println!("{i}: {}", port.port_name);
    }
    
    println!("Select a Serial Port:\t");
    let mut selection_input = String::new();
    let mut selection : usize = 0xBEEF;

    match io::stdin().read_line(&mut selection_input) {
        Ok(_) => {
            selection = selection_input.trim().parse().unwrap();
        }
        Err(_) => {},
    }
    
    if selection > ports.len().try_into().unwrap() {
        panic!("Invalid Port Selection");
    }

    let p_connect = &ports[selection].port_name;

    //let mut sport = serialport::new(p_connect, 9600).open().expect("Failed to open Port");
    let can = serial_can::SerialCAN::new(p_connect.to_string(), 9600);
    let mut msg : CANmsg = CANmsg::default();
    loop{
        msg.id = 77;
        msg.data[0] = 9;
        msg.len = 1;
        // msg.gen_crc();
        let _ = can.transmitter.send(msg);
        
        match can.receiver.try_recv() {
            Ok(msg) => {println!("ID: {:#02x}, Data: {:?}", msg.id, msg.data);}
            Err(_) => {}
        }
        thread::sleep(Duration::from_millis(200));
        //let msg = "Hello";
        //let _ = sport.write_all(msg.as_bytes());
    }
}


fn main() {
    println!("Hello, world!");
    serial_main();
}
