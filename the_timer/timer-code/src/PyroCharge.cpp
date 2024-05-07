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
    m_isActive = false;
    m_hasFired = false;
    m_pin = 0;                  //TODO: WHAT SHOULD THIS VALUE ACTUALLY BE?
    m_triggerType = ALTITUDE;
    m_value = -10000;
    m_timeOfFiring = 0;         
}

void PyroCharge::setupCharge(int8_t pin, pc_triggerType triggerType, uint32_t value) {
    m_isActive = true;
    m_pin = pin;
    m_triggerType = triggerType;
    m_value = value;
}

bool PyroCharge::canFire(const uint32_t &millisSinceApogee, const uint32_t &metersAboveGround) const {
    if(!m_isActive) return false;
    if(m_triggerType == ALTITUDE) {
        return millisSinceApogee >= m_value;
    }
    return metersAboveGround <= m_value;
}

void PyroCharge::update(const uint32_t &millisSinceApogee, const uint32_t &metersAboveGround, const uint32_t &currTimeMillis) {
    if(canFire(millisSinceApogee, metersAboveGround)) {
        if(!m_hasFired) {
            m_hasFired = true;
            m_timeOfFiring = currTimeMillis;
        }
        if(currTimeMillis < m_timeOfFiring + 1000) {
            digitalWrite(m_pin, HIGH);
        } else {
            digitalWrite(m_pin, LOW);
            m_isActive = false;
        }        
    }
}