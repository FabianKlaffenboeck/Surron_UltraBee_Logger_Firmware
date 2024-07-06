//
// Created by Fabiankla on 06/07/2024.
//

#include "LteDriver.h"


/*
 * Driver for the LteModem
 */
LteDriver::LteDriver(uint8_t rxPin, uint8_t txPin, uint8_t pwrPin, char *apn, char *gprsUser, char *gprsPass) {
    _rxPin = rxPin;
    _txPin = txPin;
    _pwrPin = pwrPin;

    _apn = apn;
    _gprsUser = gprsUser;
    _gprsPass = gprsPass;

    _apn = apn;
    _gprsUser = gprsUser;
    _gprsPass = gprsPass;

    _conLive = false;

    _modem = new TinyGsmSim7600(SerialAT);
    _client = new TinyGsmClient(*_modem);

    pinMode(_pwrPin, OUTPUT);
    SerialAT.begin(115200, SERIAL_8N1, _rxPin, _txPin);
}


/*
 * Connect to the network and authorize against the specified APN
 * return true if connection is established and false if an error occurred
 */
bool LteDriver::connect() {
    if (!_modem->init()) {
        return false;
    }

    log_d("Modem Info: ", _modem->getModemInfo());

    if (!_modem->waitForNetwork()) {
        log_e("fait to connect to network");
        return false;
    }

    if (!_modem->isNetworkConnected()) {
        return false;
    }

    log_i("Network connected");


    log_i("Connecting to ", _apn);

    if (!_modem->gprsConnect(_apn, _gprsUser, _gprsPass)) {
        log_e("apn connection failed");
        return false;
    }

    if (!_modem->isGprsConnected()) {
        return false;
    }

    log_i("apn connected");

    return true;
}

/*
 * true if connection is live and can be used
 * false if no connection is present
 */
bool LteDriver::connectionAlive() const {
    return _conLive;
}

/*
 * client object to use for further communication (http/mqtt)
 */
TinyGsmClient *LteDriver::getClient() {
    return _client;
}

/*
 * handler loop to run housekeeping tasks and check connection state
 */
void LteDriver::lteHandlerLoop() {
    _conLive = checkConnectionUp();
}

/*
 * internal function for connection checking
 */
bool LteDriver::checkConnectionUp() {
    //TODO Maybe implement some sort of ping later
    return _modem->isGprsConnected();
}

/*
 * resets the modem wia the PWRKEY pin
 */
void LteDriver::resetModem() const {
    digitalWrite(_pwrPin, LOW);
    delay(500);
    digitalWrite(_pwrPin, HIGH);
}