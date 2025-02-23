//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#ifndef FIRMWARE_MQTTHANDLER_H
#define FIRMWARE_MQTTHANDLER_H

#include <cstdlib>
#include <Client.h>
#include <PubSubClient.h>
#include "../models/VehicleData.h"

//enum publishTopics {
//    GPS_LOC = "gps_location"
//};

class MqttHandler {
public:
    MqttHandler(const char *broker, const char *deviceName, const char *userName, const char *pw, Client *client);

    void init();

    void loop();

    void setBaseTopic(const char *base);

    void subTopic(const char *topic);

    void pub(const char *publishTopic, uint8_t data);

    void pub(const char *publishTopic, VehicleData data);

private:
    PubSubClient *_mqtt;
    const char *_broker;
    const char *_deviceName;
    const char *_userName;
    const char *_pw;
    const char *_baseTopic;
    Client *_client;
    uint32_t _lastReconnectAttempt = 0;

    static void mqttCallback(char *topic, uint8_t *payload, unsigned int len);

    bool mqttConnect();

};


#endif //FIRMWARE_MQTTHANDLER_H
