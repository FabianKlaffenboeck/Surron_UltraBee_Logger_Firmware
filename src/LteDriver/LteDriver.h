//
// Created by Fabiankla on 06/07/2024.
//

#ifndef SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H
#define SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H

#define TINY_GSM_MODEM_SIM7600

#include <TinyGsmClient.h>
#include <cstdlib>

#define SerialAT Serial2
//#define TINY_GSM_RX_BUFFER 650


class LteDriver {

public:
    LteDriver(uint8_t rxPin, uint8_t txPin,  uint8_t pwrPin);

    bool connect(char *apn, char *gprsUser, char *gprsPass);

    bool connectionAlive();

    TinyGsmClient *getClient();

    void lteHandlerLoop();

private:
    uint8_t _rxPin;
    uint8_t _txPin;
    uint8_t _pwrPin;

    char *_apn;
    char *_gprsUser;
    char *_gprsPass;

    TinyGsm *_modem;
    TinyGsmClient *_client;

    bool checkConnectionUp();

    void resetModem();

};


#endif //SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H
