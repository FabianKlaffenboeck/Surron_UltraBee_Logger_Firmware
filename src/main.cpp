#include <Arduino.h>

#include <Update.h>

#include "Drivers/LteDriver/LteDriver.h"
#include "Drivers/GpsDriver/GpsDriver.h"
#include "Drivers/CanDriver/CanDriver.h"

LteDriver lteDriver = LteDriver(17, 16, 18, "webaut", "", "");
GpsDriver gpsDriver = GpsDriver(33, 32);

// MQTT details
#include <PubSubClient.h>

const char *broker = "mqtt01.klaffenboeck.eu";
const char *topicLed = "GsmClientTest/led";
const char *topicLedStatus = "GsmClientTest/ledStatus";
const char *topicInit = "GsmClientTest/init";

PubSubClient *mqtt = new PubSubClient(*lteDriver.getClient());
uint32_t lastReconnectAttempt = 0;

void mqttCallback(char *topic, byte *payload, unsigned int len) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.write(payload, len);
    Serial.println();
}

boolean mqttConnect() {
    Serial.print("Connecting to ");
    Serial.print(broker);

    // Connect to MQTT Broker

    if (!status) {
        Serial.println(" fail");
        return false;
    }

    Serial.println(" success");
    mqtt->publish(topicInit, "GsmClientTest started");
    mqtt->subscribe(topicLed);
    return mqtt->connected();
}

void setup() {
    Serial.begin(115200);

    lteDriver.connect();
    CanInit(GPIO_NUM_4, GPIO_NUM_5, 500);

    Serial.println("hello from boot");

    mqtt->setServer(broker, 1883);
    mqtt->setCallback(mqttCallback);
}

void loop() {

    if (!mqtt->connected()) {
        Serial.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        uint32_t t = millis();
        if (t - lastReconnectAttempt > 10000L) {
            lastReconnectAttempt = t;
            if (mqttConnect()) { lastReconnectAttempt = 0; }
        }
        delay(100);
        return;
    }

    mqtt->loop();


    CanHandlerLoop();
    lteDriver.lteHandlerLoop();
    gpsDriver.gpsHandlerLoop();
}