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

enum fc_state {
    IDLE, 
    PREFLIGHT, 
    ASCENDING, 
    DESCENDING, 
    POSTFLIGHT    
};

class FlightComputer {
public:
    // Needs to initialize all 3 pyro charges to channels 0, 1, 2 and set them all as inactive
    // This is so when the flight computer goes to fire each charge, it doesn't run into nothing
    FlightComputer();
    void createCharge(uint8_t channel, pc_triggerType triggerType, float value);
    void tryFiringCharges();
    void update();

private:
    float m_metersAboveGround;
    float m_secondsSinceApogee;
    fc_state m_state;
    PyroCharge m_charges[3];
};

