pub mod serial_can;
use std::time::Duration;
use std::thread;
use std::io;
use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

use serial_can::CANmsg;

pub fn serial_file_write(){
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

    let can = serial_can::SerialCAN::new(p_connect.to_string(), 9600, 99);
    let mut msg : CANmsg = CANmsg::default();
    msg.id = 0;
    
    let path = Path::new("firmware.bin");
    let display = path.display();

    let mut file = match File::open(&path) {
        Err(why) => panic!("Couldn't read {}: {}", display, why),
        Ok(file) => file,
    };

    loop{
        msg.id += 1;
        msg.data = [0; 8];

        match file.read(&mut msg.data) {
            Ok(bytes) => {
                println!("Bytes: {}", bytes);
                if bytes == 0 {
                    println!("EOF Reached");
                    break;
                }
                msg.len = bytes as u8;
            },
            Err(e)=> {
                panic!("Encountered {} while trying to read file", e);
            },
        }
        
        // println!("ID: {:#02x}, LEN: {}, Data: {}", msg.id, msg.len, std::str::from_utf8(&msg.data).unwrap());

        let _ = can.transmitter.send(msg);
        
        // match can.receiver.try_recv() {
        //     Ok(msg) => {println!("ID: {:#02x}, Data: {:?}", msg.id, msg.data);}
        //     Err(_) => {}
        // }
        let iter = can.receiver.try_iter();
        for m in iter {
            print!("ID: {}, LEN: {}, Data: {:?}", m.id, m.len, m.data);
            println!();
        }
        thread::sleep(Duration::from_millis(1000));
        //let msg = "Hello";
        //let _ = sport.write_all(msg.as_bytes());
    }
    println!("End of Program..");
}


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

    // let mut serial_port = serialport::new(p_connect.as_str(), 9600).open().expect("Failed to open Serial Port");

    let can = serial_can::SerialCAN::new(p_connect.to_string(), 9600, 99);
    let mut msg : CANmsg = CANmsg::default();
    msg.id = 0;

    loop{
        // println!("ID: {:#02x}, LEN: {}, Data: {}", msg.id, msg.len, std::str::from_utf8(&msg.data).unwrap());
        // match can.receiver.try_recv() {
        //     Ok(msg) => {println!("ID: {:#02x}, Data: {:?}", msg.id, msg.data);}
        //     Err(_) => {}
        // }

        let iter = can.receiver.try_iter();
        for m in iter {
            print!("ID: {}, LEN: {}, Data: {:?}", m.id, m.len, m.data);
            println!();
        }
        // let mut byte_buffer: [u8; 1] = [0; 1];
        // match serial_port.read(&mut byte_buffer){
        //     Ok(_) => {
        //         let _ = serial_port.write_all(&byte_buffer);
        //     },
        //     Err(_) => {}
        // }
        // thread::sleep(Duration::from_millis(1));
        //let msg = "Hello";
        //let _ = sport.write_all(msg.as_bytes());
    }
}

fn main() {
    println!("Hello, world!");
    serial_main();
}
