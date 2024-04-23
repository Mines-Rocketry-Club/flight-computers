/**
 * @file PyroCharge.cpp
 * @author William Layne (will.layne@outlook.com)
 * @brief Handles the state and firing of the pyro charges.
 * @version 0.1
 * @date 2024-04-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Arduino.h>

#ifndef PYROCHARGE_H
#define PYROCHARGE_H

class PyroCharge {
public:
    PyroCharge(uint8_t channel, float value, p_triggerType triggerType);
    void setActivationEvent();
    void tryFiring(float altitudeMeters, float timeSeconds, fc_states fc_state);
    
private:
    p_activationEvents _activationEvent = p_activationEvents::APOGEE;
    p_triggerType _triggerType;
    bool _isActive = false;
    uint8_t _channel;
    float _value;
};

#endif