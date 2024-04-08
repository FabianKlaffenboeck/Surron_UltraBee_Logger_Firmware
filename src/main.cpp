#include <Arduino.h>
#include <TinyGPS++.h>
#include "AsyncGpsHandler/AsyncGpsHandler.h"

// Serial and decoding for GPS
HardwareSerial ss(2);
TinyGPSPlus gps;

void setup() {
    Serial.begin(9600);
    Serial.print("code stared");

    ss.begin(9600, SERIAL_8N1, 32, 33);
}

void loop() {
    while (ss.available() > 0) {
        Serial.write(ss.read());
    }
}