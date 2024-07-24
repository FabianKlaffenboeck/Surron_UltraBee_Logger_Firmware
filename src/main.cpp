#include <Arduino.h>

//#include "LteDriver/LteDriver.h"
//#include "GpsDriver/GpsDriver.h"
#include "CanDriver/CanDriver.h"


//LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
//GpsDriver gpsDriver = GpsDriver(33, 32);
CanDriver canDriver = CanDriver(GPIO_NUM_4, GPIO_NUM_5, 10, CAN_SPEED_125KBPS);


void setup() {
    Serial.begin(115200);
//    lteDriver.connect();
//    canDriver.init();
}

void loop() {
    canDriver.init();
    if (canDriver.readFrame()) {

    }
//    lteDriver.lteHandlerLoop();
//    gpsDriver.gpsHandlerLoop();
}