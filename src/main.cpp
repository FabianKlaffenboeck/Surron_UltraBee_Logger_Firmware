#include <Arduino.h>

#include "LteDriver/LteDriver.h"
#include "GpsDriver/GpsDriver.h"

LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
GpsDriver gpsDriver = GpsDriver(33, 32);

void setup() {
    Serial.begin(115200);
    lteDriver.connect();
}

void loop() {
    lteDriver.lteHandlerLoop();
    gpsDriver.gpsHandlerLoop();
}