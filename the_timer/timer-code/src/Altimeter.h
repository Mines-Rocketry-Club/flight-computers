#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <Arduino.h>

/**
 * the flight computer will need from this guy:
 * the current altitude AGL
 * roughly the velocity
 * 
 * in order to deal with weird pressure effects around the transonic region, what we'll do is only have the flight computer
 * register apogee if the altimeter has been producing velocity readings close to zero for some timespan
 * 
 */

class Altimeter {
public:
    Altimeter();
    void update(const uint32_t &currentTimeMillis);
    float getAltitude();
    float getVelocity();
    float getAvgVelocity();
    //float getAcceleration();
    void setZero();

private:
    float rollingAverage(const float &newVel);

    //uint8_t address;

    uint8_t m_stepsSinceLastAvg;

    /**
     * @brief meters
     * 
     */
    float m_startingAltitude;

    /**
     * @brief meters
     * 
     */
    float m_currentAltitude;

    /**
     * @brief meters
     * 
     */
    float m_previousAltitude;

    /**
     * @brief meters per second
     * 
     */
    float m_avgVelocity;   // Needs to be kept in check with a very light rolling average

    uint32_t m_previousTime;

    float *m_rollingVelocity = new float[m_numValuesToAvg];

    const uint8_t m_numValuesToAvg = 4;
};

#endif