#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>

#include "Drivers/LteDriver/LteDriver.h"
#include "Drivers/GpsDriver/GpsDriver.h"
#include "Drivers/CanDriver/CanDriver.h"
#include "MqttHandler/MqttHandler.h"
#include "Secrets_TEMPLATE.h"
//#include "Secrets.h"
#include "CanParsers/VehicleDataParser/VehicleDataParser.h"

WebServer server(80);
const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";


VehicleDataParser vehicleDataParser;
LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
GpsDriver gpsDriver = GpsDriver(33, 32);
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
}