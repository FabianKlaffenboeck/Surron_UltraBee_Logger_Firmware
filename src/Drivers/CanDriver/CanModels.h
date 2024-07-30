//
// Created by FabianKlaffenboeck on 25/07/2024.
//

#ifndef FIRMWARE_CANMODELS_H
#define FIRMWARE_CANMODELS_H

#include <cstdlib>

struct CanMsg {
    uint32_t id = 0;
    uint32_t dlc = 0;
    uint8_t data[8];
    bool exd = false;
};



#endif //FIRMWARE_CANMODELS_H
