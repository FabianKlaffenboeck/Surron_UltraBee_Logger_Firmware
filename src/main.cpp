#include <Arduino.h>
#include <TinyGPS++.h>
#include "AsyncGpsHandler/AsyncGpsHandler.h"

// Serial and decoding for GPS
HardwareSerial ss(2);
TinyGPSPlus gps;

void setup() {
    Serial.begin(9600);
    ss.begin(9600, SERIAL_8N1, 32, 33);
}

void loop() {
    while (ss.available() > 0) {
        gps.encode(ss.read());
        if (gps.location.isUpdated()) {
            Serial.print("Latitude=");
            Serial.print(gps.location.lat(), 6);
            Serial.print("Longitude=");
            Serial.println(gps.location.lng(), 6);
        }
    }
}