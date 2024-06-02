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

#include "FlightComputer.h"

FlightComputer::FlightComputer() {
    m_state = PREFLIGHT;
    //m_metersAboveGround = 0;
    m_timeSinceApogee = 0;
}

void FlightComputer::createCharge(uint8_t channel, pc_triggerType triggerType, uint32_t value) {
    m_charges[channel].setupCharge(m_pyroPins[channel], triggerType, value);    //TODO: PLEASE DOUBLE TRIPLE CHECK THAT THE THING TRANSLATING PYRO CHANNEL TO PIN NUMBER WORKS
                                                                                // DO WE EVEN NEED IT? SHOULD WE JUST SPECIFY IT DIRECTLY IN THE IFDEFS?
}

void FlightComputer::updatePyroCharges() {
    for(int i = 0; i < m_numCharges; i++) {
        m_charges[i].update(m_timeSinceApogee, (uint32_t)altimeter.getAltitude(), m_currentTime);
    }
}

void FlightComputer::updateReadings() {
    altimeter.update(m_currentTime);
    accelerometer.update();
}

void FlightComputer::setState(fc_state newState) {
    switch(newState) {
        case PREFLIGHT:
            altimeter.setZero();
            //accelerometer.setZero(m_currentTime);
            m_state = PREFLIGHT;
            break;
        case DESCENDING:
            m_timeOfApogee = m_currentTime;
            m_state = DESCENDING;
            break;
    }
}

//TODO: this may false negative if the altimeter isn't perfect. change to be more resistant to sensor error. As well, may fuck up if the motor has intermittent thrust at first
void FlightComputer::checkForLaunch() { 
    // If we detect high vel and accel (and we're not waiting to check)
    // check again in 100ish milliseconds
    // if it's still high we launched

    if(!m_possibleLaunch && (altimeter.getVelocity() > 8 || accelerometer.getAccelMagnitude() > 50)) { // must be > 5 gees 
        m_possibleLaunch = true;
    }

    m_ticksSinceLastCheck += m_possibleLaunch;

    // Check again about 10 milliseconds later
    // TODO: slight problem: the altimeter can only actually record new data about ever 10 milliseconds. (At an OSR of 1024)
    // If this SINGLE data point is off for some reason, we delay detection another 10 milliseconds.
    // This sucks. 
    // Just check over a timespan of like 20 milliseconds and have the accelerometer do some averaging, check it has no erronius values
    if(m_ticksSinceLastCheck > 12) {    
        m_possibleLaunch = false;
        m_ticksSinceLastCheck = 0;
        if(altimeter.getVelocity() > 8 && accelerometer.getAccelMagnitude() > 50) {
            setState(ASCENDING);
        }
    }
}

void FlightComputer::checkForApogee() {
    if(abs(altimeter.getAvgVelocity()) <= 0.5 && altimeter.getVelocity() < 0) { //TODO: After we see an avg velocity <= 0.5, double check that none of the values are erronius
        setState(DESCENDING);
    }
}

void FlightComputer::checkForGroundHit() {
    if(altimeter.getAvgVelocity() <= 0.1 && m_timeSinceApogee > 3000) {
        setState(POSTFLIGHT);
    }
}

fc_state FlightComputer::getState() const {
    return m_state;
}

void FlightComputer::updateTime() {
    m_currentTime = millis();
    if(m_state == DESCENDING) {
        m_timeSinceApogee = m_currentTime - m_timeOfApogee;   // it may be better to prioritize memory over clock cycles and get rid of timeSinceApogee
    }
}