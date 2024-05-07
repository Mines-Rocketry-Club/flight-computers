#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <Arduino.h>
#include <Wire.h>

// Datasheet: 
// Address is 111011Cx, where C is the complementary value of the pin CSB [(LOW), and x is any value (?)] Therefore, addr = 11101100 = 0x76
#define ADDRESS 0x76    

#define CMD_RESET       0x1E
#define CMD_ADC_READ    0x00
#define CMD_PROM_READ   0xA0
#define CMD_CONVERT_D1  0x46    // OSR of 2048
#define CMD_CONVERT_D2  0x56    // OSR of 2048

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