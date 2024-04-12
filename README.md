# Mines Rocket Club Flight Computer Development
## Scope
The series of flight computers produced as part of this project are inteneded to be useable for any low or high power rocketry applications.
These will also span a large range of cost in order to provide flight computer capabilities for very high risk and experimental flights.
The lowest possible trim level is a CW RF emmiter intended for use with radio direction finding equipment.
The highest trim level will provide a platform for executing user space code asynchronously from the main event loop of the flight computer.
## Standards
- No features* will be lost when moving up in trim levels.
<br>
*The Screamer's CW Emmiter is lost as the other telemetry hardware on the board can be used to locate the device.

## Trim Levels
|Name         |Features                                                                         |Weight |Cost (2024) |
|-------------|---------------------------------------------------------------------------------|-------|------------|
|The Screamer | CW RF Emitter                                                                   | 5g    | 5$         |
|The Timer    | 3 Axis Accelerometer, Barometric Altimiter, CP1200 Transmitter, 4 Pyro Channels | 10g   | 20$        |
|The Guesser  | GPS, Onboard Data Logging, WIFI based configuration                             | 15g   | 40$        |
|The Knower   | User Code Execution, High Frequency Data Logging, 8 Pyro Channels               | 20g   | 60$        |

## System Architectures
### The Screamer
#### Usage
This board emits a constant signal at 146.660 MHz.
This signal can be recived by many common low-cost trancievers, such as the Baofeng UV-5RA, UV8W, and AR-5RM.
With a directional antenna any of these trancievers provides radio direction finding capabilities to the system.
#### Design
The sysem does not have a microcontroller or any other regualtion system.
It consistis of a power supply, based on the <PART NAME> and the transmitter, which is a MAX2606 IF Oscillator.
This component is not intended to be used to drive an antenna directly, however, the internal 50Ω matching and the -10dBm power output provide enough for this usage.
In order to capture the full -10dBm output, a loop antenna must be attached to ***BOTH*** of the provided antenna pads.
Bare copper wire or thin coat magnet wire is recommened for the antenna. a single loop in the wire should be adequate for most applicatons.

Due to the extremley low power consumption of the system, it can be powered parasitically off of any of the other computers in this series.
> Attachement points will be added in the next revisions of each of the boards
> - [ ] The Timer
> - [ ] The Guesser
> - [ ] The Knower

### The Timer
#### Usage
#### Design
### The Guesser
### The Knower
