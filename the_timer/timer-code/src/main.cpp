#include <Arduino.h>
#include "FlightComputer.h"

int main() {
    FlightComputer flightComputer;

    #ifdef del1 // uint32_t - milliseconds
        flightComputer.createCharge(0, DELAY, del1);
    #endif
    #ifdef alt1 // uint32_t - meters
        flightComputer.createCharge(0, ALTITUDE, alt1);
    #endif
    
    #ifdef del2 
        flightComputer.createCharge(1, DELAY, del2);
    #endif
    #ifdef alt2
        flightComputer.createCharge(1, ALTITUDE, alt2);
    #endif

    #ifdef del3
        flightComputer.createCharge(2, DELAY, del3);
    #endif
    #ifdef alt3
        flightComputer.createCharge(2, ALTITUDE, alt3);
    #endif

    #ifdef idleTime // uint32_t - milliseconds
        delay(idleTime);
    #else
        delay(15 * 1000);
    #endif

    while(true) {   //TODO: Make this run at 100tps
        flightComputer.updateTime();
        switch(flightComputer.getState()) {
            case PREFLIGHT:
                flightComputer.updateReadings();
                flightComputer.checkForLaunch();
                break;
            case ASCENDING:
                flightComputer.updateReadings();
                //storeReadings();
                flightComputer.checkForApogee();
                break;
            case DESCENDING:
                flightComputer.updateReadings();
                //storeReadings();
                flightComputer.updatePyroCharges();
                flightComputer.checkForGroundHit();
                break;
            case POSTFLIGHT:
                break;
        }
    }

    return 0;
}