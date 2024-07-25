//
// Created by FabianKlaffenboeck on 25/07/2024.
//

#ifndef FIRMWARE_CANMODELS_H
#define FIRMWARE_CANMODELS_H

#include <cstdlib>

#define CommandPacketId 0x7d1
#define ResponsePacketId 0x7d2

struct CanMsg {
    uint32_t id = 0;
    uint32_t dlc = 0;
    uint8_t data[8];
    bool exd = false;
};

struct CommandPacket {
    uint8_t _cmd;
    uint8_t _crc;
    uint8_t _size;
    uint8_t _targetId;
    uint8_t _data[5];
};

struct ResponsePacket {
    uint8_t _cmd;
    uint8_t _crc;
    uint8_t _size;
    uint8_t _senderId;
    uint8_t _data[5];
};


#endif //FIRMWARE_CANMODELS_H
