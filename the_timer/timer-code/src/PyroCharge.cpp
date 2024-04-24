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

PyroCharge::PyroCharge(int8_t channel, pc_triggerType triggerType, float value) {
    m_channel = channel;
    m_triggerType = triggerType;
    m_value = value;
}