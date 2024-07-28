//
// Created by FabianKlaffenboeck on 27/07/2024.
//

#include <Update.h>
#include <StreamString.h>
#include "UpdateHandler.h"

void UpdateHandler::init(uint64_t byteSize, uint8_t updateFs) {
    planedBytes = byteSize;
    actualBytes = 0;

    dataBytes = (uint8_t *) malloc(planedBytes * sizeof(uint8_t));
    if (!Update.begin(planedBytes)) {
        StreamString str;
        Update.printError(str);
        log_e("", str.c_str());
    }
}

void UpdateHandler::addByte(uint8_t byte) {

    if (Update.write(dataBytes, actualBytes) != actualBytes) {
        Serial.println("error during update+++++++++++++++++++++++++++++++++++++");
    }

//    dataBytes[actualBytes] = byte;
    actualBytes++;

    if (Update.hasError()) {
        Serial.println("Error----------------------------------------------------------");
    }
}


void UpdateHandler::completeUpdate(bool reboot, char *md5) {
//    if (!Update.setMD5(md5)) {
//        Update.abort();
//        log_e("ERROR: MD5 hash not valid");
//        free(dataBytes);
//        return;
//    }

    Serial.println(planedBytes);
    Serial.println(actualBytes);

    if (planedBytes != actualBytes) {
        log_e("something went terrible wrong");
    }

    Update.end(true);
    free(dataBytes);

    StreamString str;
    Update.printError(str);
    log_e("", str.c_str());

    if (reboot) {
        ESP.restart();
    }
}

