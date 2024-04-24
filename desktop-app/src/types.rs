use std::vec;
/// Standard, 3-place semantic versioning version number.
pub type Version = (u8,u8,u8);
/// Compatible flight computer implementations.
pub enum FlightComputerModel {
    Timer,
    Guesser, 
    Knower,
}
pub type FlightEvent = String;
pub type DataPoint = (f32, f32);
pub type DataEvent = (f32, FlightEvent);
pub struct FlightData {
    data: Vec<Vec<DataPoint>>,
    events: Vec<FlightEvent>,
}
pub struct HwError {
    why: &'static str,
}
pub enum PortSelectorValues{
    Port(String),
    None,
}