//
// Created by FabianKlaffenboeck on 22/07/2024.
//

#ifndef FIRMWARE_CANDRIVER_H
#define FIRMWARE_CANDRIVER_H

#include <cstdlib>
#include <ESP32CAN.h>
#include <CAN_config.h>

class CanDriver {
public:
    CanDriver(uint8_t rxPin, uint8_t txPin);

    bool init();

};


#endif //FIRMWARE_CANDRIVER_H
