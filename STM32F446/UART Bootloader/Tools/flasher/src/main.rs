use serialport;
use std::time::Duration;
use std::io;
use std::thread;

pub mod echo_test;
pub mod comms;
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
    let s2 = serial.tx.clone();

    thread::spawn(move || loop{
        let _= s2.send(43);
        thread::sleep(Duration::from_millis(20));
    });

    loop {
        let _=serial.tx.send(97);
        let _=serial.tx.send(98);
        let _=serial.tx.send(99);
        while let Ok(byte) = serial.rx.try_recv(){
            print!("{}", byte)
        }
        println!();
        thread::sleep(Duration::from_millis(200));
    }
    //echo_test::echo_test(&mut port);
    
}
