#include <Arduino.h>

#include "LteDriver/LteDriver.h"

LteDriver lteDriver = LteDriver(17, 16, 18,"","", "");

void setup() {
    Serial.begin(115200);
}

void loop() {
}