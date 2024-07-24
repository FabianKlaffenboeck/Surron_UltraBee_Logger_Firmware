#include <Arduino.h>

//#include "LteDriver/LteDriver.h"
//#include "GpsDriver/GpsDriver.h"
#include "CanDriver/CanDriver.h"

//LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
//GpsDriver gpsDriver = GpsDriver(33, 32);

unsigned long previousMillis = 0;   // will store last time a CAN Message was send
const int interval = 1000;          // interval at which send CAN Messages (milliseconds)

void setup() {
    Serial.begin(115200);

//    lteDriver.connect();
//    canDriver.init();
    CanInit(GPIO_NUM_4, GPIO_NUM_5, 10, CAN_SPEED_125KBPS);
}

void loop() {
    if (millis() - previousMillis >= interval) {
        CanMsg canMsg{};
        canMsg.id = 0xff;
        canMsg.dlc = 8;
        CanWrite(canMsg);
    }

    CanMsg canReadMsg{};
    if (CanReadFrame(&canReadMsg)) {
        Serial.println("rec");
    }

//    lteDriver.lteHandlerLoop();
//    gpsDriver.gpsHandlerLoop();
}