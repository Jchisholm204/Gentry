use serialport;
use std::time::Duration;
use std::io;
use std::thread;

pub mod echo_test;
pub mod comms;
use comms::Comms;
pub mod serial;
use serial::Serial;


fn main() {
    // List the Serial Ports
    println!("Available Serial Ports:");
    let ports = serialport::available_ports().expect("No Ports Found!");
    for (i, port) in ports.iter().enumerate() {
        println!("{i}: {}", port.port_name);
    }

    println!("Select a Serial Port:  ");

    let mut selection_input = String::new();
    let mut selection: usize = 99;
    match io::stdin().read_line(&mut selection_input) {
        Ok(_) => {
            selection = selection_input.trim().parse().unwrap();
        },
        Err(_) => (),
    }

    if selection > ports.len().try_into().unwrap() {
        panic!("Invalid Port Selection");
    }
    
    // Attempt a connection to the serial port
    let port_name = &serialport::available_ports().expect("No Ports Found!")[selection].port_name;
    println!("Connecting to port: {}", port_name);
    // Open the serial port
    let serial = Serial::new(port_name, 9600);
    
    let mut com = Comms::new(serial);
    loop {
        let mut samplePacket = comms::CommPacket{
            length: 1,
            data: [3; 16],
            crc: 0,
        };
        samplePacket.crc = Comms::compute_crc(&samplePacket);
        com.write(samplePacket);
        if com.packets_available() {
            let packet = com.read();
            println!("{}: {}", packet.length, packet.data[0]);
        }
        thread::sleep(Duration::from_millis(100));
    }
}
