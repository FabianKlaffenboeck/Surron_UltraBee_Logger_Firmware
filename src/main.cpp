#include <Arduino.h>

#include <Update.h>

//#include "LteDriver/LteDriver.h"
//#include "GpsDriver/GpsDriver.h"
#include "CanDriver/CanDriver.h"

//LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
//GpsDriver gpsDriver = GpsDriver(33, 32);


void setup() {
    Serial.begin(115200);

//    lteDriver.connect();
    CanInit(GPIO_NUM_4, GPIO_NUM_5, 500);

    Serial.println("hello from boot");
}

void loop() {
//    Serial.println("upload worked");
    CanHandlerLoop();
//    lteDriver.lteHandlerLoop();
//    gpsDriver.gpsHandlerLoop();
}