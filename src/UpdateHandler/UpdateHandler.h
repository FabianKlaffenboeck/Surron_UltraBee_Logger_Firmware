//
// Created by FabianKlaffenboeck on 27/07/2024.
//

#ifndef FIRMWARE_UPDATEHANDLER_H
#define FIRMWARE_UPDATEHANDLER_H

#include <cstdlib>

class UpdateHandler {
public:
    void init(uint64_t byteSize, uint8_t updateFs = false);

    void addByte(uint8_t byte);

    void completeUpdate(bool reboot,char* md5);

private:
    uint64_t planedBytes = 0;
    uint64_t actualBytes = 0;

    uint8_t *dataBytes;
};

#endif //FIRMWARE_UPDATEHANDLER_H
