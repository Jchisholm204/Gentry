// #![deny(unsafe_code)]
#![no_main]
#![no_std]

use core::fmt::Write;
use cortex_m_rt::entry;
use stm32f4xx_hal::interrupt;
use stm32f4xx_hal::{self as hal};
use crate::hal::{pac, prelude::*, serial::{Config, Serial}};
use cortex_m::peripheral::NVIC;
use cortex_m::interrupt::Mutex;
use heapless::{String, spsc::Queue};
use core::cell::RefCell;

use panic_probe as _; // panic handler

const BUF_SIZE : usize = 256;

static BUFFER: Mutex<RefCell<Option<String<BUF_SIZE>>>> = Mutex::new(RefCell::new(None));
static USART_QUEUE: Mutex<RefCell<Option<Queue<u8, BUF_SIZE>>>> = Mutex::new(RefCell::new(None));

#[entry]
fn main() -> ! {
    if let (Some(dp), Some(cp)) = (
        pac::Peripherals::take(),
        cortex_m::peripheral::Peripherals::take(),
    ) {
        // Set up the LED
        let gpiob = dp.GPIOB.split();
        let mut led2 = gpiob.pb7.into_push_pull_output();

        let rcc = dp.RCC.constrain();
        let clocks = rcc.cfgr.sysclk(180.MHz()).freeze();

        let mut delay = cp.SYST.delay(&clocks);

        let gpiod = dp.GPIOD.split();
        let tx_pin = gpiod.pd8.into_alternate();
        let rx_pin = gpiod.pd9.into_alternate();

        // Set up USART3 for serial communication
        let mut serial = Serial::new(
            dp.USART3,
            (tx_pin, rx_pin),
            Config::default().baudrate(9600.bps()).wordlength_8(),
            &clocks,
        )
        .unwrap();

        // Enable the RX interrupt for USART3
        serial.listen(stm32f4xx_hal::serial::Event::RxNotEmpty);

        // Enable USART3 interrupt in NVIC
        unsafe {
            NVIC::unmask(pac::Interrupt::USART3);
        }
        // Initialize the buffer and queue for storing received characters
        cortex_m::interrupt::free(|cs| {
            BUFFER.borrow(cs).replace(Some(String::new()));
            USART_QUEUE.borrow(cs).replace(Some(Queue::new()));
        });

        loop {
            // Toggle the LED
            led2.toggle();
            delay.delay_ms(500);

            // Check if we have a complete string in the buffer
            cortex_m::interrupt::free(|cs| {
                if let Some(buffer) = BUFFER.borrow(cs).borrow_mut().as_mut() {
                    if buffer.ends_with('\n') {
                        // Print the complete message and clear the buffer
                        write!(serial, "Received: {}", buffer).unwrap();
                        buffer.clear();
                    }
                }
            });
        }
    }

    loop {}
}

// USART3 interrupt handler
#[interrupt]
fn USART3() {
    cortex_m::interrupt::free(|cs| {
        let usart3 = unsafe { &*pac::USART3::ptr() };
        if usart3.sr.read().rxne().bit_is_set() {
            // Read the received character
            let received = usart3.dr.read().dr().bits() as u8;

            // Access buffer and add the received character
            if let Some(buffer) = BUFFER.borrow(cs).borrow_mut().as_mut() {
                // Only add character if there’s space in the buffer
                if buffer.push(received as char).is_err() {
                    buffer.clear(); // Clear if buffer overflows
                }
            }
        }
    });
}
