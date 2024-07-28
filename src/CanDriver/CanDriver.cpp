//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#include <esp32-hal-log.h>
#include <Arduino.h>
#include "CanDriver.h"
#include "../UpdateHandler/UpdateHandler.h"
#include "DeviceConfig.h"

UpdateHandler updateHandler = UpdateHandler();

uint8_t expectedMsC = 0;


bool msCIsValid(CommandPacket commandPacket) {
    bool valid = commandPacket._data[0] == expectedMsC;

    if (expectedMsC == 255) {
        expectedMsC = 0;
    } else {
        expectedMsC++;
    }

    return valid;
}

CommandPacket parsCanMsgToComPack(CanMsg canMsg) {
    CommandPacket commandPacket{};

    if (canMsg.id != CommandPacketId) {
        log_e("wrong CommandPackId, can not pars");
        return commandPacket;
    }

    commandPacket._cmd = canMsg.data[0];
    commandPacket._crc = (canMsg.data[1] & 0b00011111);
    commandPacket._size = ((canMsg.data[1] >> 5) & 0b00000111);
    commandPacket._targetId = canMsg.data[2];

    uint8_t dataBytePointer = 0;
    for (uint8_t i = 3; i < 8; i++) {
        commandPacket._data[dataBytePointer++] = canMsg.data[i];
    }

    return commandPacket;
}

CanMsg parsResponseToCanMsg(ResponsePacket responsePacket) {
    CanMsg canMsg{};

    canMsg.id = ResponsePacketId;
    canMsg.dlc = 8;
    canMsg.data[0] = responsePacket._cmd;
    canMsg.data[1] = ((responsePacket._crc & 0b00011111) | ((responsePacket._size & 0b00000111) << 5));
    canMsg.data[2] = responsePacket._senderId;

    uint8_t dataBytePointer = 0;
    for (uint8_t i = 3; i < 8; i++) {
        canMsg.data[i] = responsePacket._data[dataBytePointer++];
    }

    return canMsg;
}


bool CanInit(gpio_num_t rxPin, gpio_num_t txPin, int busSpeed) {


    can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(rxPin, txPin, CAN_MODE_NORMAL);
    can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
    can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

    if (can_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        log_e("can driver install not working");
    }

    if (can_start() != ESP_OK) {
        log_e("can start did not working");
    }

    return true;
}


void CanWrite(CanMsg dataFrame) {

    can_message_t message;

    message.identifier = dataFrame.id;
    message.data_length_code = dataFrame.dlc;

    if (dataFrame.exd) {
        message.flags = CAN_MSG_FLAG_EXTD;
    } else {
        message.flags = CAN_MSG_FLAG_NONE;
    }

    for (int i = 0; i < dataFrame.dlc; i++) {
        message.data[i] = dataFrame.data[i];
    }

    if (can_transmit(&message, pdMS_TO_TICKS(0)) != ESP_OK) {
        log_e("failed to queue message for transmission");
    }
}

bool CanReadFrame(CanMsg *canMsg) {

    //Wait for rxMessage to be received
    can_message_t rxMessage;
    if (can_receive(&rxMessage, pdMS_TO_TICKS(0)) != ESP_OK) {
//        log_e("Failed to receive rxMessage");
        return false;
    }

//    log_e("Message received");

    if (rxMessage.flags & CAN_MSG_FLAG_RTR) {
        return false;
    }

    canMsg->id = rxMessage.identifier;
    canMsg->exd = rxMessage.flags & CAN_MSG_FLAG_EXTD;
    canMsg->dlc = rxMessage.data_length_code;

    for (int i = 0; i < rxMessage.data_length_code; i++) {
        canMsg->data[i] = rxMessage.data[i];
    }

    return true;
}

// FIXME Code cleanup later
ResponsePacket acctOnCommand(CommandPacket cmP) {

    ResponsePacket responsePacket{};
    responsePacket._cmd = cmP._cmd;
    responsePacket._senderId = DeviceId;
    responsePacket._crc = 0;
    responsePacket._size = 0;
    responsePacket._data[0] = cmP._data[0];
    responsePacket._data[1] = 0;
    responsePacket._data[2] = 0;
    responsePacket._data[3] = 0;
    responsePacket._data[4] = 0;

    if (LIST_DEVICES == cmP._cmd) {

    }
    if (FLASH_BEGIN == cmP._cmd) {
        uint32_t expBytes = cmP._data[0] + (cmP._data[1] << 8) + (cmP._data[2] << 16) + (cmP._data[3] << 24);
        updateHandler.init(expBytes);
    }
    if (FLASH_DATA == cmP._cmd) {
        updateHandler.addBytes(cmP._data, cmP._size);
    }
    if (FLASH_END == cmP._cmd) {
        updateHandler.completeUpdate(true, "wrong");
    }

    return responsePacket;
}

// FIXME this is just a temporary function to test receiving
void actOnPack(CanMsg canMsg) {
    if (canMsg.id == CommandPacketId) {
        ResponsePacket reP = acctOnCommand(parsCanMsgToComPack(canMsg));
        CanWrite(parsResponseToCanMsg(reP));
    } else {
        // add callback function
    }
}

void CanHandlerLoop() {
    CanMsg dataFrame;
    if (CanReadFrame(&dataFrame)) {
        // TODO Maybe add callback function later
        actOnPack(dataFrame);
    }
}
