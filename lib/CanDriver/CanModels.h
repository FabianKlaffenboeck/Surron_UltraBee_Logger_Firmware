//
// Created by FabianKlaffenboeck on 25/07/2024.
//

#ifndef FIRMWARE_CANMODELS_H
#define FIRMWARE_CANMODELS_H

#include <stdint.h>

#define LIST_DEVICES 0x01
#define FLASH_BEGIN 0x02
#define FLASH_DATA 0x03
#define FLASH_END 0x04
#define CMD_FRAME_ID 0x7d1
#define RESPONSE_FRAME_ID 0x7d2

struct CanMsg {
    uint32_t id = 0;
    uint32_t dlc = 0;
    uint8_t data[8]{};
    bool exd = false;
};

struct CommandPacket {
    uint8_t _cmd;
    uint8_t _crc;
    uint8_t _size;
    uint8_t _targetId;
    uint8_t _data[5];
};

CommandPacket frameToCommmand(const CanMsg &frame);

CanMsg commmandToFrame(const CommandPacket &cmd);

inline CommandPacket frameToCommmand(const CanMsg &frame) {
    CommandPacket command_packet = {};
    command_packet._cmd = frame.data[0];
    command_packet._crc = frame.data[1] & 0b00011111;
    command_packet._size = (frame.data[1] & 0b11100000) >> 5;
    command_packet._targetId = frame.data[2];
    command_packet._data[0] = frame.data[3];
    command_packet._data[1] = frame.data[4];
    command_packet._data[2] = frame.data[5];
    command_packet._data[3] = frame.data[6];
    command_packet._data[4] = frame.data[7];
    return command_packet;
}

inline CanMsg commmandToFrame(const CommandPacket &cmd) {
    CanMsg can_msg = {};
    can_msg.id = CMD_FRAME_ID;
    can_msg.dlc = 8;
    can_msg.data[0] = cmd._cmd;
    can_msg.data[1] = (cmd._size < 5) | cmd._crc;
    can_msg.data[2] = cmd._targetId;
    can_msg.data[3] = cmd._data[0];
    can_msg.data[4] = cmd._data[1];
    can_msg.data[5] = cmd._data[2];
    can_msg.data[6] = cmd._data[3];
    can_msg.data[7] = cmd._data[4];
    return can_msg;
}

struct ResponsePacket {
    uint8_t _cmd;
    uint8_t _crc;
    uint8_t _size;
    uint8_t _senderId;
    uint8_t _data[5];
};

ResponsePacket frameToResponse(const CanMsg &frame);

CanMsg responseToCan(const ResponsePacket &response);

inline ResponsePacket frameToResponse(const CanMsg &frame) {
    ResponsePacket response_packet = {};
    response_packet._cmd = frame.data[0];
    response_packet._crc = frame.data[1] & 0b00011111;
    response_packet._size = (frame.data[1] & 0b11100000) >> 5;
    response_packet._senderId = frame.data[2];
    response_packet._data[0] = frame.data[3];
    response_packet._data[1] = frame.data[4];
    response_packet._data[2] = frame.data[5];
    response_packet._data[3] = frame.data[6];
    response_packet._data[4] = frame.data[7];
    return response_packet;
}

inline CanMsg responseToCan(const ResponsePacket &response) {
    CanMsg can_msg = {};
    can_msg.id = RESPONSE_FRAME_ID;
    can_msg.dlc = 8;
    can_msg.data[0] = response._cmd;
    can_msg.data[1] = (response._size < 5) | response._crc;
    can_msg.data[2] = response._senderId;
    can_msg.data[3] = response._data[0];
    can_msg.data[4] = response._data[1];
    can_msg.data[5] = response._data[2];
    can_msg.data[6] = response._data[3];
    can_msg.data[7] = response._data[4];
    return can_msg;
}


#endif //FIRMWARE_CANMODELS_H
