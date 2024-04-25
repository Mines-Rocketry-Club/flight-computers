/**
 * @file PyroCharge.h
 * @author William Layne
 * @brief The pyro charge object encapsulates the specific state and parameters of a charge, and manages firing.
 * @version 0.1
 * @date 2024-04-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>

#ifndef PYROCHARGE_H
#define PYROCHARGE_H

enum pc_triggerType {
    DELAY,
    ALTITUDE
};

class PyroCharge {
public:
    PyroCharge();
    void setupCharge(int8_t pin, pc_triggerType triggerType, float value);
    bool canFire(float secondsSinceApogee, float metersAboveGround) const;
    void fire();
    //void update(); BAD
private:
    bool m_isActive;
    bool m_hasFired;
    uint8_t m_pin;
    pc_triggerType m_triggerType;
    float m_value;
    uint32_t m_timeOfFiring;   // WHAT UNIT IS THIS NUMBER?? CURRENTLY MILLISECONDS. FASTER TO KEEP IN MILLIS SO WE DON'T NEED DIVISION
};

#endif