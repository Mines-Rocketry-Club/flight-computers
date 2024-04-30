#include <Altimeter.h>

Altimeter::Altimeter() {
    m_startingAltitude = 0;
    m_currentAltitude = 0;
    m_avgVelocity = 0;
    m_stepsSinceLastAvg = 0;
    for(int i = 0; i < m_numValuesToAvg; i++) {
        m_rollingVelocity[i] = 0;
    }
    //set up I2C here
}

void Altimeter::update(const uint32_t &currentTimeMillis) {  // needs to get altitude and calculate velocity
    float fakeAltitude = 7; // get altimeter reading here over I2C
    m_previousAltitude = m_currentAltitude;
    m_currentAltitude = fakeAltitude - m_startingAltitude;

    //ok we have altitude now we need to take a derivative
    // velocity = (altitude - previousAltitude) / timestep
    float newVelocity = (m_currentAltitude - m_previousAltitude) / (currentTimeMillis - m_previousTime);

    // ok now we need to take a light rolling average
    m_avgVelocity = rollingAverage(newVelocity);
}

float Altimeter::rollingAverage(const float &newVel) {
    if(m_stepsSinceLastAvg < 12) {                      // update this value depending on tickrate please so it's roughly m_numValuesToAvg/0.5 seconds (this sucks)
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
    m_startingAltitude = 7;                 // ------------------------------ get altitude here over I2C
    delay(10);
}