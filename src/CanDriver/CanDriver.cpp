//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#include "CanDriver.h"
#include "ESP32CAN.h"
#include "Arduino.h"

CAN_device_t CAN_cfg;

CommandPacket parsCanMsgToComPack(CanMsg canMsg) {
    CommandPacket commandPacket{};

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

    canMsg.data[0] = responsePacket._cmd;
    canMsg.data[1] = ((responsePacket._crc & 0b00011111) | ((responsePacket._size & 0b00000111) << 5));
    canMsg.data[2] = responsePacket._senderId;

    uint8_t dataBytePointer = 0;
    for (uint8_t i = 3; i < 8; i++) {
        canMsg.data[i] = responsePacket._data[dataBytePointer++];
    }

    return canMsg;
}


bool CanInit(uint8_t rxPin, uint8_t txPin, int rx_queue_size, CAN_speed_t busSpeed) {
    CAN_cfg.speed = CAN_SPEED_125KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_4;
    CAN_cfg.rx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_cfg));
    // Init CAN Module
    ESP32Can.CANInit();
    return true;
}


void CanWrite(CanMsg dataFrame) {
    CAN_frame_t tx_frame;

    if (dataFrame.exd) {
        tx_frame.FIR.B.FF = CAN_frame_ext;
    } else {
        tx_frame.FIR.B.FF = CAN_frame_std;
    }

    tx_frame.MsgID = dataFrame.id;
    tx_frame.FIR.B.DLC = dataFrame.dlc;
    tx_frame.data.u8[0] = dataFrame.data[0];
    tx_frame.data.u8[1] = dataFrame.data[1];
    tx_frame.data.u8[2] = dataFrame.data[2];
    tx_frame.data.u8[3] = dataFrame.data[3];
    tx_frame.data.u8[4] = dataFrame.data[4];
    tx_frame.data.u8[5] = dataFrame.data[5];
    tx_frame.data.u8[6] = dataFrame.data[6];
    tx_frame.data.u8[7] = dataFrame.data[7];
    ESP32Can.CANWriteFrame(&tx_frame);
}

bool CanReadFrame(CanMsg *canMsg) {
    CAN_frame_t rx_frame;
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
        canMsg->exd = rx_frame.FIR.B.FF == CAN_frame_ext;
        canMsg->id = rx_frame.MsgID;
        canMsg->dlc = rx_frame.FIR.B.DLC;
        canMsg->data[0] = rx_frame.data.u8[0];
        canMsg->data[1] = rx_frame.data.u8[1];
        canMsg->data[2] = rx_frame.data.u8[2];
        canMsg->data[3] = rx_frame.data.u8[3];
        canMsg->data[4] = rx_frame.data.u8[4];
        canMsg->data[5] = rx_frame.data.u8[5];
        canMsg->data[6] = rx_frame.data.u8[6];
        canMsg->data[7] = rx_frame.data.u8[7];
        return true;
    }
    return false;
}

void actOnPack(CanMsg canMsg) {
    if (canMsg.id == 0x7d1) {
        CommandPacket cmP = parsCanMsgToComPack(canMsg);

        Serial.println("------------------------------------------------");
        Serial.println("_cmd");
        Serial.println(cmP._cmd);

        Serial.println("_crc");
        Serial.println(cmP._crc);

        Serial.println("_size");
        Serial.println(cmP._size);

        Serial.println("_targetId");
        Serial.println(cmP._targetId);

        Serial.println("_data");

        for (const auto &item: cmP._data) {
            Serial.println(item);
        }

//        CanWrite(parsResponseToCanMsg());
    }
    if (canMsg.id == 0x7d2) {
        Serial.println("Resonse");
    }
}

void CanHandlerLoop() {
    CanMsg dataFrame;
    if (CanReadFrame(&dataFrame)) {
        // TODO Maybe add callback function later
        actOnPack(dataFrame);
    }
}