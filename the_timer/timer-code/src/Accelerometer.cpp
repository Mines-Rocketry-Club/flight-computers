#include <Accelerometer.h>

Accelerometer::Accelerometer() {
    m_acceleration = 0;
    m_backgroundAccel = 0;
    //TODO setup I2C
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
    float fakeAccel = 7;    //TODO: impliment I2C here ------------------------------------------------

    // ok now we need to take a light rolling average
    m_acceleration = rollingAverage(fakeAccel) - m_backgroundAccel;
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