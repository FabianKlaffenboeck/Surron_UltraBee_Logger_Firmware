//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#ifndef FIRMWARE_CANDRIVER_H
#define FIRMWARE_CANDRIVER_H

#include <cstdlib>
#include "CAN_config.h"
#include "CanModels.h"


bool CanInit(uint8_t rxPin, uint8_t txPin, int rx_queue_size, CAN_speed_t busSpeed);

void CanWrite(CanMsg dataFrame);

void CanHandlerLoop();

#endif //FIRMWARE_CANDRIVER_H
