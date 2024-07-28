//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#ifndef FIRMWARE_CANDRIVER_H
#define FIRMWARE_CANDRIVER_H

#include <cstdlib>
#include "CanModels.h"
#include "driver/can.h"


bool CanInit(gpio_num_t rxPin, gpio_num_t txPin, int busSpeed);

void CanWrite(CanMsg dataFrame);

void CanHandlerLoop();

#endif //FIRMWARE_CANDRIVER_H
