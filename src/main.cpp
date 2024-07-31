#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>

#include "Drivers/LteDriver/LteDriver.h"
#include "Drivers/GpsDriver/GpsDriver.h"
#include "Drivers/CanDriver/CanDriver.h"
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
LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
GpsDriver gpsDriver = GpsDriver(27, 26);
MqttHandler mqttHandler = MqttHandler(MQTT_BROAKER,
                                      VIN,
                                      MQTT_USERNAME,
                                      MQTT_PW,
                                      lteDriver.getClient());


void CanCallback(CanMsg canMsg) {
    VehicleBusData busData = vehicleDataParser.pars(canMsg);
    vehicleData.speed = busData.speed;
}

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.on("/", []() {
        server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
    });
    ElegantOTA.begin(&server);
    server.begin();

    lteDriver.connect();
    CanInit(GPIO_NUM_4, GPIO_NUM_5, 500);
    mqttHandler.init();
    mqttHandler.setBaseTopic(MQTT_BASETOPIC);
    CanAddCallBack(CanCallback);
}

void loop() {

    CanHandlerLoop();
    lteDriver.loop();
    gpsDriver.loop();
    mqttHandler.loop();

    server.handleClient();
    ElegantOTA.loop();

    vehicleData.lat = gpsDriver.getLocData().lat;
    vehicleData.lng = gpsDriver.getLocData().lng;
    vehicleData.altitude = gpsDriver.getLocData().altitude;
    vehicleData.time = gpsDriver.getLocData().time;

    if (millis() - lastMqttPubTime > MQTT_UPDATERATE) {
        lastMqttPubTime = millis();
        mqttHandler.pub("vehileData", vehicleData);
    }
}