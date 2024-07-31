//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#include "MqttHandler.h"
#include <ArduinoJson.h>

MqttHandler::MqttHandler(const char *broker, const char *deviceName, const char *userName, const char *pw,
                         Client *client) {
    _broker = broker;
    _deviceName = deviceName;
    _userName = userName;
    _pw = pw;
    _client = client;
    _mqtt = new PubSubClient(*_client);
}


void MqttHandler::init() {
    _mqtt->setServer(_broker, 1883);
    _mqtt->setCallback(mqttCallback);
}

void MqttHandler::loop() {
    if (!_mqtt->connected()) {
        log_e("=== MQTT NOT CONNECTED ===");
        uint32_t t = millis();
        if (t - _lastReconnectAttempt > 10000L) {
            _lastReconnectAttempt = t;
            if (mqttConnect()) {
                _lastReconnectAttempt = 0;
            }
        }
    }
    _mqtt->loop();
}

void MqttHandler::mqttCallback(char *topic, uint8_t *payload, unsigned int len) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.write(payload, len);
    Serial.println();
}

bool MqttHandler::mqttConnect() {
    log_e("Connecting to ", _broker);

    boolean status = _mqtt->connect(_deviceName, _userName, _pw);

    if (!status) {
        Serial.println(" fail");
        return false;
    }
    log_e(" success");
    return _mqtt->connected();
}

void MqttHandler::setBaseTopic(const char *base) {
    _baseTopic = base;
}

void MqttHandler::subTopic(const char *topic) {
    _mqtt->subscribe(topic);
}

void MqttHandler::pub(const char *publishTopic, uint8_t data) {

    char topicConcat[255];
    strcpy(topicConcat, _baseTopic);
    strcat(topicConcat, publishTopic);

    char dataArray[8];
    dtostrf(data, 1, 2, dataArray);
    _mqtt->publish(topicConcat, dataArray);
}

void MqttHandler::pub(const char *publishTopic, VehicleData data) {
    char topicConcat[128];
    strcpy(topicConcat, _baseTopic);
    strcat(topicConcat, publishTopic);

    JsonDocument doc;
    char buffer[1024];

    doc["speed"] = data.vehicleSpeed;
    doc["time"] = data.time;
    doc["lat"] = data.lat;
    doc["lng"] = data.lng;
    doc["altitude"] = data.altitude;

    serializeJson(doc, buffer);

    _mqtt->publish(topicConcat, buffer);
}



