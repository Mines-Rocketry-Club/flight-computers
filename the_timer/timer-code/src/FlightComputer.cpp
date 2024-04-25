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
    m_state = IDLE;
    m_metersAboveGround = 0;
    m_secondsSinceApogee = 0;
    //m_startingAltitudeMeters = getAltimiterData();
}

void FlightComputer::createCharge(uint8_t channel, pc_triggerType triggerType, float value) {
    m_charges[channel].setupCharge(m_pyroPins[channel], triggerType, value);    // PLEASE DOUBLE TRIPLE CHECK THAT THE THING TRANSLATING PYRO CHANNEL TO PIN NUMBER WORKS
                                                                                // DO WE EVEN NEED IT? SHOULD WE JUST SPECIFY IT DIRECTLY IN THE IFDEFS?
}

void FlightComputer::updatePyroCharges() {
    for(uint8_t i = 0; i < m_numCharges; i++) {
        if(m_charges[i].canFire(m_metersAboveGround, m_secondsSinceApogee)) {
            m_charges[i].fire();
        }
    }
}

void FlightComputer::updateReadings() {
    // GET DATA HERE
}
