/**
 * @file PyroCharge.cpp
 * @author William Layne
 * @brief Handles the state and firing of the pyro charges.
 * @version 0.1
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include "PyroCharge.h"

PyroCharge::PyroCharge() {
    m_state = DISABLED;
    m_pin = 0;                  //TODO: WHAT SHOULD THIS VALUE ACTUALLY BE?
    m_triggerType = DELAY;
    m_value = -10000;
    m_timeOfFiring = 0;         
}

void PyroCharge::setupCharge(int8_t pin, pc_triggerType triggerType, uint32_t value) {
    m_state = ARMED;
    m_pin = pin;
    m_triggerType = triggerType;
    m_value = value;
}

bool PyroCharge::canFire(const uint32_t &millisSinceApogee, const uint32_t &metersAboveGround) const {
    if(m_state == DISABLED) return false;
    if(m_triggerType == DELAY) {
        return millisSinceApogee >= m_value;
    }
    return metersAboveGround <= m_value;
}

void PyroCharge::update(const uint32_t &millisSinceApogee, const uint32_t &metersAboveGround, const uint32_t &currTimeMillis) {
    if(canFire(millisSinceApogee, metersAboveGround)) {
        if(m_state != FIRING) {
            m_state = FIRING;
            m_timeOfFiring = currTimeMillis;
        }
        if(currTimeMillis < m_timeOfFiring + 1000) { // TODO: The charge remains active but only for 1 second here. Find an appropriate value based on pyro charge oscope data
            digitalWrite(m_pin, HIGH);
        } else {
            digitalWrite(m_pin, LOW);
            m_state = DISABLED;
        }
    }
}

pc_state PyroCharge::getState() const {
    return m_state;
}