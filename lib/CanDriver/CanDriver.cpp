//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#include <Arduino.h>
#include "CanDriver.h"
#include <esp32-hal-log.h>
#include <driver/can.h>
#include <hal/can_types.h>


static can_callback_t callbacks[MAX_CALLBACKS];
static int callback_count = 0;

bool CanInit(gpio_num_t rxPin, gpio_num_t txPin, int busSpeed) {
    can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(rxPin, txPin, CAN_MODE_NORMAL);
    can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
    can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

    if (can_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        log_e("can driver install not working");
    }

    if (can_start() != ESP_OK) {
        log_e("can start did not working");
    }

    return true;
}


void CanWrite(const CanMsg &dataFrame) {
    can_message_t message;

    message.identifier = dataFrame.id;
    message.data_length_code = dataFrame.dlc;

    if (dataFrame.exd) {
        message.flags = CAN_MSG_FLAG_EXTD;
    } else {
        message.flags = CAN_MSG_FLAG_NONE;
    }

    for (int i = 0; i < dataFrame.dlc; i++) {
        message.data[i] = dataFrame.data[i];
    }

    if (can_transmit(&message, pdMS_TO_TICKS(0)) != ESP_OK) {
        log_e("failed to queue message for transmission");
    }
}

bool CanRegisterCallback(can_callback_t cb) {
    if (callback_count < MAX_CALLBACKS) {
        callbacks[callback_count++] = cb;
    } else {
        log_e("Error: Cannot register more callbacks.\n");
        return false;
    }
    return true;
}

bool CanReadFrame(CanMsg *canMsg) {
    //Wait for rxMessage to be received
    can_message_t rxMessage;

    if (can_receive(&rxMessage, pdMS_TO_TICKS(0)) != ESP_OK) { return false; }
    if (rxMessage.flags & CAN_MSG_FLAG_RTR) { return false; }

    canMsg->id = rxMessage.identifier;
    canMsg->exd = rxMessage.flags & CAN_MSG_FLAG_EXTD;
    canMsg->dlc = rxMessage.data_length_code;

    for (int i = 0; i < rxMessage.data_length_code; i++) {
        canMsg->data[i] = rxMessage.data[i];
    }

    return true;
}

void CanHandlerLoop() {
    CanMsg dataFrame;
    if (CanReadFrame(&dataFrame)) {
        for (int i = 0; i < callback_count; i++) {
            callbacks[i](dataFrame);
        }
    }
}
