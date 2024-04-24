/**
 * @file FlightComputer.cpp
 * @author William Layne
 * @brief The main Thinker file. Manages the state for the whole computer.
 * @version 0.1
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <PyroCharge.h>

FlightComputer::FlightComputer() {
    m_state = IDLE;
    for(int i = 0; i < 3; i++) {
        createCharge(i, ALTITUDE, -10000);
    }
}

void FlightComputer::createCharge(uint8_t channel, pc_triggerType triggerType, float value) {
    m_charges[channel] = PyroCharge(channel, triggerType, value);
}

void FlightComputer::tryFiringCharges() {
    for(int i = 0; i < 3; i++) {
        if(m_charges[i].canFire(m_metersAboveGround, m_secondsSinceApogee)) {
            m_charges[i].fire();
        }
    }
}