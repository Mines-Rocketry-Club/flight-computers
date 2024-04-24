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
    PyroCharge(bool isActive, int8_t channel, pc_triggerType triggerType, float value);
    bool canChargeFire(float secondsSinceApogee, float metersAboveGround) const;
    void fire();
private:
    bool m_isActive;
    const uint8_t m_channel;
    const pc_triggerType m_triggerType;
    const float m_value;
    uint32_t m_timeOfFiring;
};

#endif