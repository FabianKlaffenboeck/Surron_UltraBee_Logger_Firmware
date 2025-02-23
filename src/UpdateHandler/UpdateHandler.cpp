//
// Created by FabianKlaffenboeck on 27/07/2024.
//

#include <Update.h>
#include "UpdateHandler.h"

void UpdateHandler::init(void (*tx_fun)(const CanMsg &dataFrame)) {
    canWriteHandler = tx_fun;
}

bool UpdateHandler::start(uint16_t expectedBytes) {
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
        log_e("Update Begin Error: %s\n", Update.errorString());
        return false;
    }

    canWriteHandler(responseToCan({
        ._cmd = 0,
        ._crc = 0,
        ._size = 0,
        ._senderId = 0,
        ._data = {},
    }));

    return true;
}

bool UpdateHandler::rxHandler(const CanMsg &rx_frame) {
    if (rx_frame.id != CMD_FRAME_ID) {
        return false;
    }

    const CommandPacket cmd = frameToCommmand(rx_frame);

    switch (cmd._cmd) {
        case FLASH_BEGIN:
            start(0);
            break;
        case FLASH_DATA:
            for (int i = 0; i < cmd._size; i++) {
                addByte(cmd._data[i], false);
            };
            canWriteHandler(responseToCan({
                ._cmd = 0,
                ._crc = 0,
                ._size = 0,
                ._senderId = 0,
                ._data = {},
            }));
            break;
        case FLASH_END:
            finish();
            break;

        default:
            return false;
    }

    return true;
}

bool UpdateHandler::addByte(uint8_t data, bool lastByte = false) {
    if (Update.write(&data, 1) != 1) {
        log_e("Write Error: %s\n", Update.errorString());
        Update.abort();
        return false;
    }

    _recivedBytes++;

    return true;
}

bool UpdateHandler::finish(bool reboot) {
    if (!Update.end(true)) {
        log_e("Update Failed: %s\n", Update.errorString());
        Update.abort();
        return false;
    }

    canWriteHandler(responseToCan({
        ._cmd = 0,
        ._crc = 0,
        ._size = 0,
        ._senderId = 0,
        ._data = {},
    }));

    if (reboot) {
        log_e("Update Success! Rebooting...");
        delay(1000);
        ESP.restart();
    }
    return true;
}
