#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>

#include "Drivers/LteDriver/LteDriver.h"
#include "Drivers/GpsDriver/GpsDriver.h"
#include "Drivers/CanDriver/CanDriver.h"
#include "Drivers/IO_ReadDriver/IO_ReadDriver.h"
#include "MqttHandler/MqttHandler.h"
//#include "Secrets_TEMPLATE.h"
#include "Secrets.h"
#include "CanParsers/VehicleDataParser/VehicleDataParser.h"
#include "models/VehicleData.h"
#include "Settings.h"

WebServer server(80);
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

unsigned long lastMqttPubTime = 0;

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

void setupOTA() {
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.on("/", []() {
        server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
    });
    ElegantOTA.begin(&server);
    server.begin();
}

void otaLoop() {
    server.handleClient();
    ElegantOTA.loop();
}

void setup() {
    Serial.begin(115200);

    setupOTA();

    ioReadDriver.init(32);
    lteDriver.connect();
    CanInit(GPIO_NUM_4, GPIO_NUM_5, 500);
    mqttHandler.init();
    mqttHandler.setBaseTopic(MQTT_BASETOPIC);
    CanAddCallBack(CanCallback);
}

void loop() {

    otaLoop();

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