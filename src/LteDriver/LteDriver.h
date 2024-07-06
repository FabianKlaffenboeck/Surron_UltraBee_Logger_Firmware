//
// Created by Fabiankla on 06/07/2024.
//

#ifndef SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H
#define SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H

#include <cstdlib>


class LteDriver {

public:
    LteDriver(uint8_t rxPin, uint8_t txPin, char *apn, char *gprsUser, char *gprsPass);

    bool connect();

    bool connectionAlive();

private:
    uint8_t rxPin;
    uint8_t txPin;
    char *apn;
    char *gprsUser;
    char *gprsPass;
};


#endif //SURRON_ULTRABEE_FIRMWARE_LTEDRIVER_H
