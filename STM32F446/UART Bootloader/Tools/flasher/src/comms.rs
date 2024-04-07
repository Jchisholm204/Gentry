use crate::serial::Serial;


const PACKET_DATA_LEN : usize = 16;
const PACKET_LENGTH_LEN : usize = 16;
const PACKET_CRC_LEN : usize = 16;
const PACKET_LEN : usize = PACKET_LENGTH_LEN + PACKET_DATA_LEN + PACKET_CRC_LEN;

const COMMS_RETX : u8 = 0x34;
const COMMS_ACC : u8 = 0x35;

const RX_BUFFER_LEN: usize = 64;


#[derive(Default, Clone, Copy)]
struct CommPacket {
    length : u8,
    data   : [u8; PACKET_DATA_LEN],
    crc    : u8,
}

enum CommState{
    Length,
    Data,
    CRC,
}

struct Comms {
    port:                  Serial,
    state:                 CommState,
    rx_byte_count:         usize,
    temp_packet:           CommPacket,
    retx_packet:           CommPacket,
    acc_packet:            CommPacket,
    last_tx_packet:        CommPacket,
    rx_buffer:             [CommPacket; RX_BUFFER_LEN],
    rx_buffer_read_index:  usize,
    rx_buffer_write_index: usize,
    rx_buffer_mask:        usize,
}

impl Comms {
    pub fn new(com_port: Serial) -> Self {
        let mut retx_packet = CommPacket::default();
        retx_packet.length = 1;
        retx_packet.data[0] = COMMS_RETX;
        retx_packet.crc = Self::compute_crc(&retx_packet);

        let mut acc_packet = CommPacket::default();
        acc_packet.length = 1;
        acc_packet.data[0] = COMMS_ACC;
        retx_packet.crc = Self::compute_crc(&acc_packet);

        let packet_buffer = [CommPacket::default(); RX_BUFFER_LEN];

        return Comms {
            port: com_port,
            state: CommState::Length,
            rx_byte_count: 0,
            temp_packet: CommPacket::default(),
            retx_packet,
            acc_packet,
            last_tx_packet: CommPacket::default(),
            rx_buffer: packet_buffer,
            rx_buffer_read_index: 0,
            rx_buffer_write_index: 0,
            rx_buffer_mask: RX_BUFFER_LEN - 1,
        }
    }

    pub fn tick(&mut self) {
        while let Ok(byte) = self.port.rx.try_recv(){
            match self.state{
                CommState::Length => {
                    self.temp_packet.length = byte;
                    self.state = CommState::Data;
                }
                CommState::Data => {
                    self.temp_packet.data[self.rx_byte_count] = byte;
                    self.rx_byte_count += 1;
                    if self.rx_byte_count >= self.temp_packet.length as usize {
                        self.rx_byte_count = 0;
                        self.state = CommState::CRC;
                    }
                }
                CommState::CRC => {
                    self.temp_packet.crc = byte;
                    if self.temp_packet.crc != Self::compute_crc(&self.temp_packet) {
                        self.write(self.retx_packet);
                        self.state = CommState::Length;
                        break;
                    }
                    if Self::packet_is(self.temp_packet, COMMS_RETX) {
                        self.write(self.last_tx_packet);
                        self.state = CommState::Length;
                        break;
                    }
                    if Self::packet_is(self.temp_packet, COMMS_ACC) {
                        self.state = CommState::Length;
                    }
                    let next_write_index = (self.rx_buffer_write_index + 1) & self.rx_buffer_mask;
                    if next_write_index == self.rx_buffer_read_index {
                        self.state = CommState::Length;
                        break;
                    }
                    self.rx_buffer[self.rx_buffer_write_index] = self.temp_packet;
                    self.rx_buffer_write_index = next_write_index;
                    self.write(self.acc_packet);
                    self.state = CommState::Length;
                    break;
                }
            }
        }

    }

    pub fn packets_available(&self) -> bool {
        return !(self.rx_buffer_read_index == self.rx_buffer_write_index);
    }

    fn packet_is(packet: CommPacket, is: u8) -> bool{
        if packet.length != 1 { return false; }
        if packet.data[0] != is {return false; }
        return true;
    }

    pub fn write(&mut self, packet: CommPacket){
        let _ = self.port.tx.send(packet.length);
        for byte in packet.data {
            let _ = self.port.tx.send(byte);
        }
        let _ = self.port.tx.send(packet.length);
    }

    pub fn read(&mut self) -> CommPacket{
        return CommPacket::default();
    }

    fn compute_crc(packet: &CommPacket) -> u8{
        let mut bytes = [0u8; PACKET_LEN-PACKET_CRC_LEN];
        bytes[0] = packet.length;
        for i in 0..(PACKET_LEN - PACKET_CRC_LEN){
            bytes[i+1] = packet.data[i];
        }
        return Self::crc8(&bytes);
    }

    fn crc8(data: &[u8]) -> u8{
        let mut crc : u8 = 0;
        for i in data {
            crc ^= i;
            for _ in 0..8 {
                if crc & 0x80 > 0 {
                    crc = (crc << 1) ^ 0x7;
                }
                else {
                    crc <<= 1;
                }
            }
        }
        return crc;
    }
    
}

