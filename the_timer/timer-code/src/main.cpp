#include <Arduino.h>
#include <FlightComputer.h>

int main() {
    #ifdef del1
    flightComputer.createCharge(0, DELAY, del1);
    #endif
    #ifdef alt1
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

    #ifdef idleTime
    delay(static_cast<uint32_t>(idleTime) * 1000);
    #else
    delay(15 * 1000);
    #endif

    FlightComputer flightComputer;

    while(true) {
        flightComputer.update();
    }

    return 0;
}
