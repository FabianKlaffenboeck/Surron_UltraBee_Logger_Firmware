//
// Created by Fabiankla on 06/07/2024.
//

#include "LteDriver.h"

LteDriver::LteDriver(uint8_t rxPin, uint8_t txPin, char *apn, char *gprsUser, char *gprsPass) {
    _rxPin = rxPin;
    _txPin = txPin;
    _apn = apn;
    _gprsUser = gprsUser;
    _gprsPass = gprsPass;

}

bool LteDriver::connect() {
    return false;
}

bool LteDriver::connectionAlive() {
    return false;
}

TinyGsmClient *LteDriver::getClient() {
    return _client;
}

void LteDriver::lteHandlerLoop() {

}

bool LteDriver::checkConnectionUp() {
    return false;
}
