#include "Accelerometer.h"

Accelerometer::Accelerometer() {
    m_acceleration = 0;
    m_backgroundAccel = 0;
    
    // Set sample rate to 128/s (we don't need any faster)
    Wire.beginTransmission(ADDRESS);    
    Wire.write(REG_SR);
    Wire.write(0b000);  //TODO: find a readable to specify these values because just randomly using 0b000 is incomprehensible and requires datasheet knowledge
    Wire.endTransmission();

    // Set range select to +- 16 gees
    Wire.beginTransmission(ADDRESS);
    Wire.write(REG_RANGE);
    Wire.write(0b011);
    Wire.endTransmission();

    // Set mode from STANDBY to WAKE
    Wire.beginTransmission(ADDRESS);
    Wire.write(REG_MODE);
    Wire.write(0b01);
    Wire.endTransmission();
}

float Accelerometer::getAcceleration() {
    return m_acceleration;
}

void Accelerometer::setZero(const uint32_t &startTimeMillis) {
    for(int i = 0; i < m_numValuesToAvg * 2; i++) {
        delay(10);
        update();
        delay(10);
    }
    m_backgroundAccel = m_acceleration;
}

void Accelerometer::update() {  // needs to get altitude and calculate velocity
    Wire.beginTransmission(ADDRESS);
    Wire.write(XOUT_LSB);
    Wire.endTransmission();
    Wire.requestFrom(ADDRESS, 2);

    // Get the values for X, Y, and Z.
    // This code only (probably) works because each of these registers is consecutive in memory, and the chip is specifically meant to increment registers on consecutive reads.
    // TODO: check that this is formatted correctly to read consecutive registers
    int16_t xVal = (Wire.read() << 8) | Wire.read();    Wire.endTransmission();
    int16_t yVal = (Wire.read() << 8) | Wire.read();    Wire.endTransmission();
    int16_t zVal = (Wire.read() << 8) | Wire.read();    Wire.endTransmission();

    // Get re-oriented values for acceleration (corrected to meters per second)
    m_xAccel = (-xVal - yVal)/(sqrt(2) * 2048 * 9.81);  // TODO: How much precision does this provide? What does this compile to? Make sure we're not wasting space on a double
    m_yAccel = zVal / (2048 * 9.81);
    m_zAccel = (-xVal + yVal)/(sqrt(2) * 2048 * 9.81);

    m_accelMagnitude = sqrt(pow(xVal, 2) + pow(yVal, 2) + pow(zVal, 2));
}

float Accelerometer::rollingAverage(float newAccel) {
    for(uint8_t i = 1; i < m_numValuesToAvg; i++) {
        m_rollingAccel[i - 1] = m_rollingAccel[i];
    }
    m_rollingAccel[m_numValuesToAvg - 1] = newAccel;

    float sum = 0;
    for(uint8_t i = 0; i < m_numValuesToAvg; i++) {
        sum += m_rollingAccel[i];
    }

    sum /= m_numValuesToAvg;
    return sum;
}