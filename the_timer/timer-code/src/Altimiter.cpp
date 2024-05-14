#include "Altimeter.h"

Altimeter::Altimeter() {
    m_startingAltitude = 0;
    m_currentAltitude = 0;
    m_avgVelocity = 0;
    m_stepsSinceLastAvg = 0;
    for(int i = 0; i < m_numValuesToAvg; i++) {
        m_rollingVelocity[i] = 0;
    }

    //TODO: set up I2C here
    // need to connect then read in factory calibration data to coeficcients array
    // send reset command
    // next send command to see each subsequent prom address and store value
    m_i2c->begin();
    m_i2c->beginTransmission(ADDRESS);
    m_i2c->write(CMD_RESET);
    m_i2c->endTransmission(true);
    delay(3);

    for(uint8_t i = 0; i < 8; i++) {
        //get value from prom address
            //this looks like: send command
            //request bits
            //read bits
        m_i2c->beginTransmission(ADDRESS);
        m_i2c->write(CMD_ADC_READ + i * 2);
        m_i2c->endTransmission(true);

        m_i2c->requestFrom(ADDRESS, 2);             // Request 2 bytes from chip
        m_coefficients[i] = m_i2c->read() << 8;     // Store first byte of coefficient
        m_coefficients[i] += m_i2c->read();         // Store 2nd byte of coefficient
    }
}

uint32_t Altimeter::m_calculateAltitude() {
    // read digital pressure and temperature data
    // Get D1
    m_i2c->beginTransmission(ADDRESS);
    m_i2c->write(CMD_CONVERT_D1);
    m_i2c->endTransmission(true);

    delay(4); //TODO: is this enough delay?? use 1024 OSR instead please

    m_i2c->requestFrom(ADDRESS, 3); // this uh, kind of sucks
    m_D1 = m_i2c->read() << 16;
    m_D1 += m_i2c->read() << 8;
    m_D1 += m_i2c->read();

    // Get D2
    m_i2c->beginTransmission(ADDRESS);
    m_i2c->write(CMD_CONVERT_D2);
    m_i2c->endTransmission(true);

    delay(4);

    m_i2c->requestFrom(ADDRESS, 3);
    m_D2 = m_i2c->read() << 16;
    m_D2 += m_i2c->read() << 8;
    m_D2 += m_i2c->read();

    //cool what now
    //we need to calculate all the bullshit

    // Calculate temperature
    int32_t dT = m_D2 - m_coefficients[4] * (2 << 8);
    int32_t TEMP = 2000 + dT * (m_coefficients[5] >> 23);

    // Calculate temperature compensated pressure
    int64_t OFF = (m_coefficients[1] << 17) + ((m_coefficients[3] * dT) >> 6);
    int64_t SENS = (m_coefficients[0] << 16) + ((m_coefficients[2] * dT) >> 7);
    int32_t P = (m_D1 * (SENS >> 21) - OFF) >> 15;
    // we probably don't need 2nd order temperature compensation right
    // right guys haha
    // TODO

    // ok cool now we have to somehow convert pressure to altitude
    // this sucks

    //TODO: formula to convert pressure to altitude

    return 1;
}

void Altimeter::update(const uint32_t &currentTimeMillis) {
    float newAltitude = m_calculateAltitude();
    m_previousAltitude = m_currentAltitude;
    m_currentAltitude = newAltitude - m_startingAltitude;

    //ok we have altitude now we need to take a derivative
    // velocity = (altitude - previousAltitude) / timestep
    float newVelocity = (m_currentAltitude - m_previousAltitude) / (currentTimeMillis - m_previousTime);

    // ok now we need to take a light rolling average
    m_avgVelocity = rollingAverage(newVelocity);
}

float Altimeter::rollingAverage(const float &newVel) {
    if(m_stepsSinceLastAvg < 12) {                      //TODO: update this value depending on tickrate please so it's roughly m_numValuesToAvg/0.5 seconds (this sucks)
        m_stepsSinceLastAvg++;
        return m_avgVelocity;
    }
    m_stepsSinceLastAvg = false;

    for(uint8_t i = 1; i < m_numValuesToAvg; i++) {
        m_rollingVelocity[i - 1] = m_rollingVelocity[i];
    }
    m_rollingVelocity[m_numValuesToAvg - 1] = newVel;

    float sum = 0;
    for(uint8_t i = 0; i < m_numValuesToAvg; i++) {
        sum += m_rollingVelocity[i];
    }

    sum /= m_numValuesToAvg;
    return sum;
}

float Altimeter::getAltitude() {
    return m_currentAltitude;
}

float Altimeter::getVelocity() {
    return m_rollingVelocity[m_numValuesToAvg - 1];
}

float Altimeter::getAvgVelocity() {
    return m_avgVelocity;
}

void Altimeter::setZero() {
    delay(10);
    m_startingAltitude = m_calculateAltitude();
    delay(10);
}