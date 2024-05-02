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

#ifndef PYROCHARGE_H
#define PYROCHARGE_H

#include <Arduino.h>

enum pc_triggerType {
    DELAY,
    ALTITUDE
};

class PyroCharge {
public:
    PyroCharge();
    void setupCharge(int8_t pin, pc_triggerType triggerType, float value);
    void update(const float &secondsSinceApogee, const float &metersAboveGround, const uint32_t &currTimeMillis);   // Pass by reference should reduce transfer costs
    //bool canFire(const float &secondsSinceApogee, const float &metersAboveGround) const;
private:
    bool canFire(const float &secondsSinceApogee, const float &metersAboveGround) const;
    bool m_isActive;
    bool m_hasFired;
    uint8_t m_pin;
    pc_triggerType m_triggerType;
    float m_value;
    uint32_t m_timeOfFiring;
};

#endif