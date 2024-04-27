#include <Altimeter.h>

Altimeter::Altimeter() {
    m_startingAltitude = -1;
    m_currentAltitude = 0;
    m_velocity = 0;
    //set up I2C here
}

void Altimeter::update(uint32_t currentTimeMillis) {  // needs to get altitude and calculate velocity
    float fakeAltitude = 7; // get altimeter reading here over I2C
    m_previousAltitude = m_currentAltitude;
    m_currentAltitude = fakeAltitude - m_startingAltitude;

    //ok we have altitude now we need to take a derivative
    // velocity = (altitude - previousAltitude) / timestep
    float newVelocity = (m_currentAltitude - m_previousAltitude) / (currentTimeMillis - m_previousTime);

    // ok now we need to take a light rolling average
    m_velocity = rollingAverage(newVelocity);
}

float Altimeter::rollingAverage(float newVel) {
    for(uint8_t i = 1; i < m_numValuesToAvg; i++) {
        m_rollingVelocity[i - 1] = m_rollingVelocity[i];    // DOUBLE CHECK M_ROLLINGVELOCITY GETS INITIALIZED TO ALL ZERO WHEN CREATED
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
    return m_velocity;
}