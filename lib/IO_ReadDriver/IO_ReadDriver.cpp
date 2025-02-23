//
// Created by FabianKlaffenboeck on 02/08/2024.
//

#include "IO_ReadDriver.h"
#include <Arduino.h>

void IO_ReadDriver::init(uint8_t accPin) {
    _accPin = accPin;
    pinMode(_accPin, INPUT);
}

bool IO_ReadDriver::getACC_State() {
    return digitalRead(_accPin);
}
