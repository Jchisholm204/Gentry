//! Demonstrate the use of a blocking `Delay` using the SYST (sysclock) timer.

#![deny(unsafe_code)]
#![allow(clippy::empty_loop)]
#![no_main]
#![no_std]

use core::fmt::Write;

// Halt on panic
use panic_probe as _; // panic handler

use cortex_m_rt::entry;
use stm32f4xx_hal::{self as hal, gpio::gpiod, hal_02::blocking::serial};

use crate::hal::{pac, prelude::*, serial::{Config, Serial}};

#[entry]
fn main() -> ! {
    if let (Some(dp), Some(cp)) = (
        pac::Peripherals::take(),
        cortex_m::peripheral::Peripherals::take(),
    ) {
        // Set up the LED. On the Nucleo-446RE it's connected to pin PA5.
        let gpiob = dp.GPIOB.split();
        let mut led = gpiob.pb7.into_push_pull_output();


        // Set up the system clock. We want to run at 48MHz for this one.
        let rcc = dp.RCC.constrain();
        let clocks = rcc.cfgr.sysclk(180.MHz()).freeze();

        // Create a delay abstraction based on SysTick
        let mut delay = cp.SYST.delay(&clocks);
        
        let gpiod = dp.GPIOD.split();
        let tx_pin = gpiod.pd8.into_alternate();
        let rx_pin = gpiod.pd9.into_alternate();

    
        let mut serial = Serial::new(
            dp.USART3, 
            (tx_pin, rx_pin), 
            Config::default().baudrate(9600.bps()).wordlength_8(),
            &clocks
        ).unwrap();

        loop {
            // On for 1s, off for 1s.
            led.toggle();
            writeln!(serial, "Hello World");

            match serial.read() {
                Ok(byte) => {
                    serial.write(byte).unwrap();
                },
                Err(_) => {},
            };
            
            // serial.write_str("Hello World\n");
            delay.delay_ms(500);
        }
    }

    loop {}
}