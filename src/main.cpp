#include <Arduino.h>
#include "AsyncGpsHandler/AsyncGpsHandler.h"

AsyncGpsHandler asyncGpsHandler(32, 33);

void setup() {
    Serial.begin(115200);
}

void loop() {
    asyncGpsHandler.loop();
}