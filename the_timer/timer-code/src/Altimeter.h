#include <Arduino.h>

#ifndef ALTIMETER_H
#define ALTIMETER_H
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
    void update(uint32_t currentTimeMillis);
    float getAltitude();
    float getVelocity();
    //float getAcceleration();
    void setStartingAltitude();

private:
    //uint8_t address;

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
    float m_velocity;   // Needs to be kept in check with a very light rolling average

    uint32_t m_previousTime;

    //float *m_rollingVelocity = (float*)malloc(m_numValuesToAvg * sizeof(float));
    float *m_rollingVelocity = new(float[m_numValuesToAvg]); // CHECK THAT THIS INITIALIZES VALUES TO ZERO SO FIRST ROLLING AVG DOESN'T BREAK

    float rollingAverage(float newVel);

    const uint8_t m_numValuesToAvg = 4;
};

#endif