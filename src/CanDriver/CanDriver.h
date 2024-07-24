//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#ifndef FIRMWARE_CANDRIVER_H
#define FIRMWARE_CANDRIVER_H

#include <cstdlib>
#include "CAN_config.h"

struct CanMsg {
    uint32_t id = 0;
    uint32_t dlc = 0;
    uint8_t data[8];
};


bool CanInit(uint8_t rxPin, uint8_t txPin, int rx_queue_size, CAN_speed_t busSpeed);

void CanHandlerLoop();

void CanWrite(CanMsg dataFrame);

bool CanReadFrame(CanMsg *canMsg);

#endif //FIRMWARE_CANDRIVER_H
