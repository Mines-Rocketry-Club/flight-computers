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

#ifndef FLIGHTCOMPUTER_H
#define FLIGHTCOMPUTER_H

#include <Arduino.h>
#include <PyroCharge.h>
#include <Accelerometer.h>
#include <Altimeter.h>

enum fc_state {
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
    void setState(fc_state newState);
    void update();
    void checkForLaunch();
    void checkForApogee();

private:
    const uint8_t m_numCharges = 3;
    const uint8_t m_pyroPins[3] = {0, 1, 2};    // TRANSLATE "CHANNEL" TO PIN NUMBER ON THE CHIP. MIGHT BE A BAD IDEA
    float m_metersAboveGround;
    float m_secondsSinceApogee;
    fc_state m_state;
    PyroCharge m_charges[3];
    Altimeter altimeter;
    Accelerometer accelerometer;
    uint32_t m_currentTime;
    bool m_possibleLaunch;
    uint32_t m_scheduledCheck;
};

#endif
