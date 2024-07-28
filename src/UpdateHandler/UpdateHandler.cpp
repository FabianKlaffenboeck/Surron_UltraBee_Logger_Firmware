//
// Created by FabianKlaffenboeck on 27/07/2024.
//

#include <Update.h>
#include <StreamString.h>
#include "UpdateHandler.h"

void UpdateHandler::init(uint64_t byteSize, uint8_t updateFs) {
    planedBytes = byteSize;
    actualBytes = 0;
    dataBytes = (uint8_t *) malloc(byteSize * sizeof(uint8_t));
    if (!Update.begin(UPDATE_SIZE_UNKNOWN, updateFs ? U_SPIFFS : U_FLASH)) {
        StreamString str;
        Update.printError(str);
        log_e("", str.c_str());
    }
}

void UpdateHandler::addByte(uint8_t byte) {
    dataBytes[actualBytes] = byte;
    actualBytes++;
}

void UpdateHandler::completeUpdate(bool reboot,char* md5) {
    if (!Update.setMD5(md5)) {
        Update.abort();
        log_e("ERROR: MD5 hash not valid");
        free(dataBytes);
        return;
    }

    Update.write(dataBytes, actualBytes);
    Update.end(true);
    free(dataBytes);

    if (reboot) {
        ESP.restart();
    }
}
