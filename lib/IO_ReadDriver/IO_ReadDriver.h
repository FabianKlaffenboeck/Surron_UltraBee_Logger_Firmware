//
// Created by FabianKlaffenboeck on 02/08/2024.
//

#ifndef FIRMWARE_IO_READDRIVER_H
#define FIRMWARE_IO_READDRIVER_H

#include "stdlib.h"

class IO_ReadDriver {
public:
    void init(uint8_t accPin);

    bool getACC_State();

private:
    uint8_t _accPin;
};


#endif //FIRMWARE_IO_READDRIVER_H
