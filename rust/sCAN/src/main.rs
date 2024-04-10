pub mod serial_can;

pub fn serial_main(){
    println!("Available Serial Ports");
    let ports = serialport::available_ports().expect("No Serial Ports Found");
    for (i, port) in ports.iter().enumerate() {
        println!("{i}: {}", port.port_name);
    }

    let p_connect = &ports[1].port_name;

    let mut sport = serialport::new(p_connect, 9600).open().expect("Failed to open Port");
    loop{
        let msg = "Hello";
        let _ = sport.write_all(msg.as_bytes());
    }
}


fn main() {
    println!("Hello, world!");
    serial_main();
}
