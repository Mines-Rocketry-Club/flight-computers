use log::warn;
use serialport::{self, SerialPortInfo, TTYPort};
use crate::types;

/// Contains the functionality for communicating with the devices to
/// set flight parameters and update firmware. 
/// Since some computers do 
trait FlightComputer {
    fn get_fw_version(&self) -> types::Version;
    fn get_hw_version(&self) -> types::Version;
    fn write_firmware(&mut self) -> Result<(), types::HwError>;
    fn read_flightdata(&self) -> Result<types::FlightData, types::HwError>;
    fn clear_flightdata(&mut self) -> Result<types::FlightData, types::HwError>;
}

pub fn get_avalible_ports() -> Vec<String> {
    let detected_ports = serialport::available_ports();
    let mut port_names = Vec::<String>::new();
    let detected_ports = match detected_ports {
        Ok(ports) => ports,
        Err(_) => Vec::<SerialPortInfo>::new(),
    };
    for p in detected_ports {
        port_names.push(p.port_name)
    }
    return port_names;
}