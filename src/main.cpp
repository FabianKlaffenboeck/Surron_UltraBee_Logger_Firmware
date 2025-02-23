#include <Arduino.h>
#include <VehicleDataParser/VehicleDataParser.h>

#include "../lib/LteDriver/LteDriver.h"
#include "../lib/GpsDriver/GpsDriver.h"
#include "../lib/CanDriver/CanDriver.h"
#include "../lib/IO_ReadDriver/IO_ReadDriver.h"
#include "UpdateHandler/UpdateHandler.h"

#include "MqttHandler/MqttHandler.h"
#include "Secrets_TEMPLATE.h"
#include "Secrets.h"
#include "models/VehicleData.h"
#include "Settings.h"

unsigned long lastMqttPubTime = 0;

UpdateHandler update_handler;
VehicleData vehicleData;
VehicleDataParser vehicleDataParser;
IO_ReadDriver ioReadDriver;
LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
GpsDriver gpsDriver = GpsDriver(27, 26);
MqttHandler mqttHandler = MqttHandler(MQTT_BROAKER,
                                      VIN,
                                      MQTT_USERNAME,
                                      MQTT_PW,
                                      lteDriver.getClient());


void CanCallback(CanMsg canMsg) {
    vehicleDataParser.pars(canMsg);
}


void setup() {
    Serial.begin(115200);

    CanInit(GPIO_NUM_4, GPIO_NUM_5, 500);

    update_handler.init(CanWrite);

    CanRegisterCallback([](CanMsg msg) {
        update_handler.rxHandler(msg);
    });

    CanRegisterCallback([](CanMsg msg) {
        CanCallback(msg);
    });

    ioReadDriver.init(32);
    lteDriver.connect();
    mqttHandler.init();
    mqttHandler.setBaseTopic(MQTT_BASETOPIC);
}

void loop() {
    CanHandlerLoop();
    lteDriver.loop();
    gpsDriver.loop();
    mqttHandler.loop();

    VehicleBusData busData = vehicleDataParser.getLatestData();

    vehicleData.lat = gpsDriver.getLatestData().lat;
    vehicleData.lng = gpsDriver.getLatestData().lng;
    vehicleData.altitude = gpsDriver.getLatestData().altitude;
    vehicleData.time = gpsDriver.getLatestData().time;
    vehicleData.vehicleSpeed = busData.speed;
    vehicleData.ACC = ioReadDriver.getACC_State();


    if (millis() - lastMqttPubTime > MQTT_UPDATERATE) {
        lastMqttPubTime = millis();
        mqttHandler.pub("vehileData", vehicleData);
    }
}
