#ifndef ALTIMETER_H
#define ALTIMETER_H

#include <Arduino.h>
#include <Wire.h>

// Address is 111011Cx, where C is the complementary value of the pin CSB [(LOW), and x is any value (?)] Therefore, addr = 11101100 = 0x76
#define ADDRESS 0x76            // TODO: These might be "better" as variables?

#define CMD_RESET       0x1E
#define CMD_ADC_READ    0x00
#define CMD_PROM_READ   0xA0    // Starting prom address. Each subsequent address is 2 bits after the last.
#define CMD_CONVERT_D1  0x44    // OSR of 1024
#define CMD_CONVERT_D2  0x54    // OSR of 1024

class Altimeter {
public:
    Altimeter();

    /**
     * @brief Updates the internal altitude and velocity readings of the altimeter. Run every tick.
     * 
     * @param currentTimeMillis - Current time in milliseconds
     */
    void update(const uint32_t &currentTimeMillis);

    /**
     * @brief Gets the current calculated altitude above GROUND level in meters.
     * 
     * @return float - cast to a uint32_t when passing to a pyro charge
     */
    float getAltitude();

    /**
     * @brief Gets the current calculated velocity. No averaging is used here.
     * 
     * @return float 
     */
    float getVelocity();

    /**
     * @brief Gets the rolling average of the velocity across about the past 0.5 seconds.
     * 
     * @return float 
     */
    float getAvgVelocity();

    /**
     * @brief Zeroes the altitude of the object. Run only during the flight computer's preflight state.
     * 
     */
    void setZero();

    

private:
    /**
     * @brief Calculates the current calculated altitude above GROUND level in meters based off of pressure and temperature.
     * 
     * @return float
     */
    float m_calculateAltitude();

    /**
     * @brief Creates a continuous rolling average of velocity across m_numValuesToAvg per roughly 0.5 seconds. Returns the average in meters. 
     * 
     * @param newVel 
     * @return float 
     */
    float m_rollingAvgVelocity(const float &newVel);

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

    //TODO: The library I was referencing used a private TwoWire pointer... I don't know why. I think that just using Wire.doSomething() should be OK for single threaded stuff?
    //TwoWire *m_i2c;

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
    float m_avgVelocity; 

    uint32_t m_previousTime;

    float *m_rollingVelocity = new float[m_numValuesToAvg];

    const uint8_t m_numValuesToAvg = 4;
};

#endif