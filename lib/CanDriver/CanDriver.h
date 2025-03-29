//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#ifndef FIRMWARE_CANDRIVER_H
#define FIRMWARE_CANDRIVER_H

#include "CanModels.h"
#include <driver/can.h>

#define MAX_CALLBACKS  10

typedef void (*can_callback_t)(CanMsg dataFrame);

bool CanInit(gpio_num_t rxPin, gpio_num_t txPin, int busSpeed);

bool CanRegisterCallback(can_callback_t cb);

void CanWrite(const CanMsg &dataFrame);

void CanHandlerLoop();

#endif //FIRMWARE_CANDRIVER_H
