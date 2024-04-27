/**
 * @file FlightComputer.h
 * @author William Layne
 * @brief The flight computer creates and manages pyro charges. -----------------------------------------------------------------------
 * @version 0.1
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>
#include <PyroCharge.h>

#ifndef FLIGHTCOMPUTER_H
#define FLIGHTCOMPUTER_H

enum fc_state {
    IDLE, 
    PREFLIGHT, 
    ASCENDING, 
    DESCENDING, 
    POSTFLIGHT    
};

class FlightComputer {
public:
    FlightComputer();
    void createCharge(uint8_t channel, pc_triggerType triggerType, float value);
    void updatePyroCharges();
    void updateReadings();
    void storeReadings(); // IMPLIMENT LATER
    //Detecting launch needs to be a function that keeps a rolling average over a small timespan of acceleration and altitude data
    //and only if those go above background levels by some threshold is a launch detected 

private:
    const uint8_t m_numCharges = 3;
    const uint8_t m_pyroPins[3] = {0, 1, 2};    // TRANSLATE "CHANNEL" TO PIN NUMBER ON THE CHIP. MIGHT BE A BAD IDEA
    float m_startingAltitudeMeters;
    float m_metersAboveGround;
    float m_secondsSinceApogee;
    fc_state m_state;
    PyroCharge m_charges[3];
};

#endif
