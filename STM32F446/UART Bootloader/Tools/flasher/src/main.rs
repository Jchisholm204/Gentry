use serialport;
use std::time::Duration;
use std::io;

pub mod echo_test;

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
    let mut port = serialport::new(port_name, 9600).open().expect("Failed to open port");
    let _ = port.set_timeout(Duration::from_millis(10));

    echo_test::echo_test(&mut port);
    
}
