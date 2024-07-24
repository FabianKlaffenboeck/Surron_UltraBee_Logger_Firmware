//
// Created by FabianKlaffenboeck on 22/07/2024.
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

class CanDriver {
public:
    CanDriver(uint8_t rxPin, uint8_t txPin, int rx_queue_size, CAN_speed_t busSpeed);

    void  canHandlerLoop();

    bool init();

    void canWrite(CanMsg dataFrame);

    bool readFrame();


private:
    uint8_t _rxPin;
    uint8_t _txPin;
    uint8_t _rx_queue_size;
    CAN_speed_t _busSpeed;
    CAN_device_t CAN_cfg;
};


#endif //FIRMWARE_CANDRIVER_H
