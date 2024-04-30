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
#include <FlightComputer.h>

FlightComputer::FlightComputer() {
    m_state = PREFLIGHT;
    m_metersAboveGround = 0;
    m_secondsSinceApogee = 0;
}

void FlightComputer::createCharge(uint8_t channel, pc_triggerType triggerType, float value) {
    m_charges[channel].setupCharge(m_pyroPins[channel], triggerType, value);    // PLEASE DOUBLE TRIPLE CHECK THAT THE THING TRANSLATING PYRO CHANNEL TO PIN NUMBER WORKS
                                                                                // DO WE EVEN NEED IT? SHOULD WE JUST SPECIFY IT DIRECTLY IN THE IFDEFS?
}

void FlightComputer::updatePyroCharges() {
    for(int i = 0; i < m_numCharges; i++) {
        m_charges[i].update(m_secondsSinceApogee, m_metersAboveGround, m_currentTime);
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
            accelerometer.setZero(m_currentTime);
            m_state = PREFLIGHT;
            break;
    }
}

void FlightComputer::checkForLaunch() {
    // basically need to set state to ascending if we see acceleration down greater than background by some amount, say 4 gees
    // and also see some largeish velocity from the altimeter. say, >8 m/s
    // maybe we make these values configurable in the future

    // if high gees and velocity are detected
    // schedule a check for 100ms from now
    // if they're still high, set state to ascending
    if(altimeter.getVelocity() > 8 && accelerometer.getAcceleration() > 4 && !m_possibleLaunch) {
        m_possibleLaunch = true;
        m_scheduledCheck = m_currentTime + 100;
    }

    if(m_currentTime > m_scheduledCheck) {
        m_possibleLaunch = false;
        if(altimeter.getVelocity() > 8 && accelerometer.getAcceleration() > 4) {
            setState(ASCENDING);
        }
    }
}

void FlightComputer::checkForApogee() {
    if(abs(altimeter.getAvgVelocity()) <= 0.5 && altimeter.getVelocity() < 0) {
        setState(DESCENDING);
    }
}

// this should probably be changed so this whole sequence gets handled in main
void FlightComputer::update() {
    m_currentTime = millis();
    switch(m_state) {
        case PREFLIGHT:
            updateReadings();
            checkForLaunch();
            break;
        case ASCENDING:
            updateReadings();
            //storeReadings();
            checkForApogee();
            break;
        case DESCENDING:
            updateReadings();
            //storeReadings();
            updatePyroCharges();
            //checkForGroundHit();
            break;
        case POSTFLIGHT:
            break;
    }
}
