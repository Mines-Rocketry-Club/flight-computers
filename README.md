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
|The ?????????| 3 Axis Accelerometer, Barometric Altimiter, CP1200 Transmitter, 4 Pyro Channels | 10g   | 20$        |
|The Guesser  | GPS, Onboard Data Logging, WIFI based configuration                             | 15g   | 40$        |
|The Knower   | User Code Execution, High Frequency Data Logging, 8 Pyro Channels               | 20g   | 60$        |
