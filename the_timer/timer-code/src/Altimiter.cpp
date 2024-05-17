#include "Altimeter.h"

Altimeter::Altimeter() {
    m_startingAltitude = 0;
    m_currentAltitude = 0;
    m_avgVelocity = 0;
    m_stepsSinceLastAvg = 0;
    for(int i = 0; i < m_numValuesToAvg; i++) {
        m_rollingVelocity[i] = 0;
    }

    // need to connect then read in factory calibration data to coeficcients array
    // send reset command
    // next send command to see each subsequent prom address and store value
    Wire.begin();
    Wire.beginTransmission(ADDRESS);
    Wire.write(CMD_RESET);
    Wire.endTransmission(true);
    delay(3);

    for(uint8_t i = 0; i < 8; i++) {
        //get value from prom address
            //this looks like: send command
            //request bits
            //read bits
        Wire.beginTransmission(ADDRESS);
        Wire.write(CMD_ADC_READ + i * 2);
        Wire.endTransmission(true);

        Wire.requestFrom(ADDRESS, 2);             // Request 2 bytes from chip
        m_coefficients[i] = Wire.read() << 8;     // Store first byte of coefficient
        m_coefficients[i] += Wire.read();         // Store 2nd byte of coefficient
    }
}

float Altimeter::m_calculateAltitude() {
    // Read digital pressure and temperature data
    
    Wire.beginTransmission(ADDRESS);  // Get D1
    Wire.write(CMD_CONVERT_D1);
    Wire.endTransmission(true);

    delay(3);                           // This delay is only enough if you're using an OSR of 1024 or less

    Wire.requestFrom(ADDRESS, 3);
    m_D1 = Wire.read() << 16;
    m_D1 += Wire.read() << 8;
    m_D1 += Wire.read();

    
    Wire.beginTransmission(ADDRESS);  // Get D2
    Wire.write(CMD_CONVERT_D2);
    Wire.endTransmission(true);

    delay(3);

    Wire.requestFrom(ADDRESS, 3);
    m_D2 = Wire.read() << 16;
    m_D2 += Wire.read() << 8;
    m_D2 += Wire.read();

    // Start calculations

    // Calculate temperature
    int32_t dT = m_D2 - m_coefficients[4] * (2 << 8);
    int32_t TEMP = 2000 + dT * (m_coefficients[5] >> 23);

    // Second order temperature compensation
    int32_t T2 = 0;
    int32_t OFF2 = 0;
    int32_t SENS2 = 0;
    if(TEMP < 2000) {
        T2 = pow(dT, 2) / pow(2, 31);
        OFF2 = (61 * pow(TEMP - 2000, 2)) / pow(2, 4);
        SENS2 = 2 * pow(TEMP - 2000, 2);

        if(TEMP < -1500) {
            OFF2 += 15 * pow(TEMP + 1500, 2);
            SENS2 += 8 * pow(TEMP + 1500, 2);
        }
    }
    TEMP -= T2;

    // Calculate temperature compensated pressure
    // TODO: double check that these bit shifts don't cause data to fall off (where we don't want)
    int64_t OFF = (m_coefficients[1] << 17) + ((m_coefficients[3] * dT) >> 6) - OFF2;
    int64_t SENS = (m_coefficients[0] << 16) + ((m_coefficients[2] * dT) >> 7) - SENS2;
    int32_t P = (m_D1 * (SENS >> 21) - OFF) >> 15;

    float finalPressure = (float)P / 100;

    // Convert pressure to altitude using barometric formula
    float P_ref = 1013.25;
    float finalAltitude = 44307.69396 * (1 - pow(P/P_ref, 0.190284)) - m_startingAltitude;  // TODO: please cross-check that this formula outputs the correct values

    return finalAltitude;
}

void Altimeter::update(const uint32_t &currentTimeMillis) {
    float newAltitude = m_calculateAltitude();
    m_previousAltitude = m_currentAltitude;
    m_currentAltitude = newAltitude;

    //ok we have altitude now we need to take a derivative
    // velocity = (altitude - previousAltitude) / timestep
    float newVelocity = (m_currentAltitude - m_previousAltitude) / (currentTimeMillis - m_previousTime);

    // ok now we need to take a light rolling average
    m_avgVelocity = m_rollingAvgVelocity(newVelocity);
}

float Altimeter::m_rollingAvgVelocity(const float &newVel) {
    if(m_stepsSinceLastAvg < 12) {      // TODO: update this value depending on tickrate please so it's roughly m_numValuesToAvg/0.5 seconds (this sucks)
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

// TODO: this should REALLY average over a few values across a couple seconds or something just in case there is a gust of wind while measuring
void Altimeter::setZero() {
    delay(10);
    m_startingAltitude = m_calculateAltitude();
    delay(10);
}