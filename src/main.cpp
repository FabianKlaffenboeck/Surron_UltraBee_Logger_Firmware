#include <Arduino.h>
#include <TinyGPS++.h>
#include "AsyncGpsHandler/AsyncGpsHandler.h"

// Serial and decoding for GPS
HardwareSerial ss(1);
TinyGPSPlus gps;

void setup() {
    Serial.begin(9600);
    Serial.print("code stared");

    ss.begin(9600);
    ss.setPins(32, 33);
}

void loop() {
    while (ss.available() > 0) {
        gps.encode(ss.read());
        if (gps.location.isUpdated()) {
            Serial.print("Latitude= ");
            Serial.println(gps.location.lat(), 6);
            Serial.print("Longitude= ");
            Serial.println(gps.location.lng(), 6);
            Serial.print("Date= ");
            Serial.println(gps.date.day(), 6);
        }
    }
}
