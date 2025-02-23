//
// Created by FabianKlaffenboeck on 05/09/2024.
//

#ifndef ESP32CANUPLOAD_UPDATEHANDLER_H
#define ESP32CANUPLOAD_UPDATEHANDLER_H

#include <cstdint>
#include <CanModels.h>

#define BUFFER_SIZE 1024


class UpdateHandler {
public:
    void init(void (*tx_fun)(const CanMsg &dataFrame));

    bool start(uint16_t expectedBytes);

    bool rxHandler(const CanMsg &rx_frame);

    bool addByte(uint8_t data, bool lastByte);

    bool finish(bool reboot = true);

private:
    uint16_t _expectedBytes = 0;
    uint16_t _recivedBytes = 0;

    void (*canWriteHandler)(const CanMsg &dataFrame) = nullptr;
};


#endif //ESP32CANUPLOAD_UPDATEHANDLER_H
