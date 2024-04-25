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
#include <PyroCharge.h>

PyroCharge::PyroCharge() {
    m_isActive = false;
    m_pin = 0;                  // WHAT SHOULD THIS VALUE ACTUALLY BE?
    m_triggerType = ALTITUDE;
    m_value = -10000;
    m_timeOfFiring = 0;         
}

bool PyroCharge::canFire(float secondsSinceApogee, float metersAboveGround) const {
    if(!m_isActive) return;
    if(m_triggerType == ALTITUDE) {
        return secondsSinceApogee >= m_value;
    }
    return metersAboveGround <= m_value;
}

/**
 * THIS FUNCTION IS CRINGE
 * IT'S UNCLEAR THAT IT NEEDS TO BE CALLED EVERY TICK THAT YOU WANT THE CHARGE TO BE ACTIVE IF IT IS TO EVER TURN OFF
 * whatever this needs to be running Saturday
 */
void PyroCharge::fire() {
    if(!m_hasFired) {
        m_hasFired = true;
        m_timeOfFiring = millis(); // THESE DATATYPES MAY NOT PLAY NICELY UNLESS YOU'RE AWARE OF WHAT YOU'RE DOING
    }
    if(millis() < m_timeOfFiring + 1000) {
        digitalWrite(m_pin, HIGH);
    } else {
        digitalWrite(m_pin, LOW);
        m_isActive = false;
    }
}