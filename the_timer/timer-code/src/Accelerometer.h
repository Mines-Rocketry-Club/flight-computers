#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

// 7-bit device ID: 0x4C - 0b1001100
// 8-bit address - write: 0x98
// 8-bit address - read: 0x99
/**
 * The last bit determines if reading or writing to the device. 0 = write, 1 = read
 * Writing to a register:
 *      START | Device ID + 0x0 | Register address | Register data to write | STOP
 * Reading from a register:
 *      START | Device ID + 0x0 | Register address | RESTART | Device ID + 0x1 | <read data byte here> | NAK | STOP
 * 
 * the Wire library should just handle this
*/

/**
 * After the IMU powers up, it will be in the standby state by default. 
 * In this state we can write to registers so we will want to first use this state to write any parameters we want. 
 * Next, switch it to WAKE because it's useless for anything else in STANDBY as far as I know
 * 
 * Registers to modify:
 *      Sample rate - Addr: 0x08 - Value: 0b000 - Speed of 128s/second - default
 *      Interrupts - Not being used currently, "If polling is used, no need for the interrupt registers to be set up"
 *      Range select control - Addr: 0x20 - Value: 0b011, +- 16 gees
 *      Mode Register - Addr: 0x07 - Value: 0b01, WAKE
 * 
 */

#define IMU_ADDRESS 0x4C
#define REG_MODE 0x07
#define REG_SR 0x08
#define REG_RANGE 0x20

#define XOUT_LSB 0x0D
#define XOUT_MSB 0x0E

#define YOUT_LSB 0x0F
#define YOUT_MSB 0x10

#define ZOUT_LSB 0x11
#define ZOUT_MSB 0x12

#include <Arduino.h>
#include <Wire.h>

class Accelerometer {
public:
    Accelerometer();
    void update();
    float getAccelMagnitude();
    float rollingAverage(float newAccel);   // Does acceleration actually need to be averaged?
    // it probably should be. In a specific way, though. We should really be trying to filter out short lived peaks... this would hopefully prevent false positives from
    // being bumped or dropped
private:
    //float m_accelMagnitude; // TODO: should just be calculated on the fly
    //float m_backgroundAccel; // TODO: we don't need this
    float *m_rollingAccel = new float[m_numValuesToAvg];
    const uint8_t m_numValuesToAvg = 4;
    float m_xAccel;
    float m_yAccel;
    float m_zAccel;
};

#endif
