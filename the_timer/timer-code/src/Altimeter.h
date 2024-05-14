#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <Arduino.h>
#include <Wire.h>

//THE PROCEDURE
    /**
     * (on startup only) read calibration data and store it
     * 
     * from then on any time we want data, 
     * read D1 (digital pressure), D2 (digital temperature)
     * calculate temperature: dT and TEMP
     * calculate temperature compensated pressure: OFF, SENS, and P
     * 
     * next, use these values in whatever they need to be used by
     */
// We want a total time to do the entire calculation to be < 10ms. We'll need to read the ADC twice and conversion time at OSR 2048 is 4.54ms max which should work?

// Datasheet: 
// Address is 111011Cx, where C is the complementary value of the pin CSB [(LOW), and x is any value (?)] Therefore, addr = 11101100 = 0x76
#define ADDRESS 0x76    

#define CMD_RESET       0x1E
#define CMD_ADC_READ    0x00
#define CMD_PROM_READ   0xA0    // Starting prom address. Each subsequent address is 2 bits after the last.
#define CMD_CONVERT_D1  0x46    // OSR of 2048
#define CMD_CONVERT_D2  0x56    // OSR of 2048

class Altimeter {
public:
    Altimeter();
    void update(const uint32_t &currentTimeMillis);
    float getAltitude();
    float getVelocity();
    float getAvgVelocity();
    void setZero();

    

private:
    uint32_t m_calculateAltitude();

    float rollingAverage(const float &newVel);

    //TODO: why doesnt the fucking doxygen newline work
    /**
     * @brief Factory calibration coefficients. \n 
     * | C1 | Pressure sensitivity                  | SENS_T1   | \n 
     * | C2 | Pressure offset                       | OFF_T1    | \n 
     * | C3 | Temp. coeff. of pressure sensitivity  | TCS       | \n 
     * | C4 | Temp. coeff. of pressure offset       | TCO       | \n 
     * | C5 | Reference temperature                 | T_REF     | \n 
     * | C6 | Temp. coeff. of the temperature       | TEMPSENS  | \n 
     */
    uint16_t m_coefficients[6];

    uint32_t m_D1;    // Digital pressure. may rename
    uint32_t m_D2;    // Digital temperature. may rename

    // TODO -----------------
    // these values and the three below are used for doing math and are derived from the constants along with D1 and D2.
    // these should probably be moved to a function and be declared inside the call...? 
    // basically, these aren't technically necessary (mostly) and we want to save on as much ram as possible here
    int32_t m_dT;
    int32_t m_TEMP;

    // Jesus, int64s 
    int64_t m_OFF;
    int64_t m_SENS;
    int32_t m_P;

    //TODO: does wire even need to be a declared variable.? it can be used statically right but why doesn't the library??
    TwoWire *m_i2c;

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