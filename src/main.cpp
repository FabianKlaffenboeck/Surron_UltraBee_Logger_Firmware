#include <Arduino.h>

#include "Drivers/LteDriver/LteDriver.h"
#include "Drivers/GpsDriver/GpsDriver.h"
#include "Drivers/CanDriver/CanDriver.h"
#include "MqttHandler/MqttHandler.h"
//#include "Secrets_TEMPLATE.h"
#include "Secrets.h"

LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
GpsDriver gpsDriver = GpsDriver(33, 32);
MqttHandler mqttHandler = MqttHandler(MQTT_BROAKER,
                                      VIN,
                                      MQTT_USERNAME,
                                      MQTT_PW,
                                      lteDriver.getClient());

uint32_t _lastReconnectAttempt = 0;


void setup() {
    Serial.begin(115200);

    lteDriver.connect();
    CanInit(GPIO_NUM_4, GPIO_NUM_5, 500);
    mqttHandler.init();

    mqttHandler.setBaseTopic(MQTT_BASETOPIC);
}

void loop() {

    uint32_t t = millis();
    if (t - _lastReconnectAttempt > 10000L) {
        mqttHandler.pub("GPS_LOC", 255);
        _lastReconnectAttempt = t;
    }

    CanHandlerLoop();
    lteDriver.loop();
    gpsDriver.loop();
    mqttHandler.loop();
}