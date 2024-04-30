#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>

class Accelerometer {
public:
    Accelerometer();
    void update();
    float getAcceleration();
    void setZero(const uint32_t &startTimeMillis);
    float rollingAverage(float newAccel);
private:
    float m_acceleration;
    float m_backgroundAccel;
    float *m_rollingAccel = new float[m_numValuesToAvg];
    const uint8_t m_numValuesToAvg = 4;
};

#endif
