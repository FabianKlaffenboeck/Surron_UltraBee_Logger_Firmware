//
// Created by Fabiankla on 06/07/2024.
//

#ifndef SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H
#define SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H

#define TINY_GSM_MODEM_SIM7600

#include <TinyGsmClient.h>
#include <cstdlib>


class LteDriver {

public:
    LteDriver(uint8_t rxPin, uint8_t txPin, char *apn, char *gprsUser, char *gprsPass);

    bool connect();

    bool connectionAlive();

    TinyGsmClient *getClient();

    void lteHandlerLoop();

private:
    uint8_t _rxPin;
    uint8_t _txPin;
    TinyGsm *_modem;
    TinyGsmClient *_client;
    char *_apn;
    char *_gprsUser;
    char *_gprsPass;

    bool checkConnectionUp();

};


#endif //SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H
