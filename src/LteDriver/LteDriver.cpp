//
// Created by Fabiankla on 06/07/2024.
//

#include "LteDriver.h"

LteDriver::LteDriver(uint8_t rxPin, uint8_t txPin, uint8_t pwrPin) {
    _rxPin = rxPin;
    _txPin = txPin;
    _pwrPin = pwrPin;

    pinMode(_pwrPin, OUTPUT);
    _modem = modem(SerialAT);
}

bool LteDriver::connect(char *apn, char *gprsUser, char *gprsPass) {
    _apn = apn;
    _gprsUser = gprsUser;
    _gprsPass = gprsPass;

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

void LteDriver::resetModem() {
    digitalWrite(_pwrPin, LOW);
    delay(500);
    digitalWrite(_pwrPin, HIGH);
}
