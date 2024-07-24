//
// Created by FabianKlaffenboeck on 22/07/2024.
//

#include "CanDriver.h"
#include "ESP32CAN.h"

CanDriver::CanDriver(uint8_t rxPin, uint8_t txPin, int rx_queue_size, CAN_speed_t busSpeed) {
    _rxPin = rxPin;
    _txPin = txPin;
    _rx_queue_size = rx_queue_size;
    _busSpeed = busSpeed;
}

bool CanDriver::init() {
    CAN_cfg.speed = _busSpeed;
    CAN_cfg.tx_pin_id = _txPin;
    CAN_cfg.rx_pin_id = _rxPin;
    CAN_cfg.rx_queue = xQueueCreate(_rx_queue_size, sizeof(CanMsg));
//    ESP32Can.CANInit();
    return true;
}

void CanDriver::canHandlerLoop() {

}

void CanDriver::canWrite(CanMsg dataFrame) {

}

bool CanDriver::readFrame() {
    return false;
}
