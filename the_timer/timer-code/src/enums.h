/**
 * @brief DEPRECATED
 * 
 */

/**
 * @brief State of the timer
 * IDLE         - Board does nothing for a set period of time before switching to PREFLIGHT state
 * PREFLIGHT    - Board anticipates launch and will switch to ASCENDING state once the rocket lifts off
 * ASCENDING    - Board tracks altitude and records data until apogee, firing any necessary pyro charges
 * DESCENDING   - Board tracks altitude and records data until ground hit, firing any necessary pyro charges
 * POSTFLIGHT   - Board does nothing
 */
enum class fc_states {
    IDLE, 
    PREFLIGHT, 
    ASCENDING, 
    DESCENDING, 
    POSTFLIGHT    
};

enum p_activationEvents {
    APOGEE = 0,
    LAUNCH = 1
};

enum class p_triggerType {
    DELAY,
    ALTITUDE
};