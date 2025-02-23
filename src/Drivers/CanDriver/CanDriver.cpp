//
// Created by FabianKlaffenboeck on 24/07/2024.
//

#include <esp32-hal-log.h>
#include "CanDriver.h"

bool _hasCallback = false;

void (*cb_func)(CanMsg canMsg);

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


void CanWrite(CanMsg dataFrame) {
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

bool CanReadFrame(CanMsg *canMsg) {
    //Wait for rxMessage to be received
    can_message_t rxMessage;
    if (can_receive(&rxMessage, pdMS_TO_TICKS(0)) != ESP_OK) {
        return false;
    }

    if (rxMessage.flags & CAN_MSG_FLAG_RTR) {
        return false;
    }

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
        if (_hasCallback) {
            cb_func(dataFrame);
        }
    }
}

void CanAddCallBack(void(*fun_ptr)(CanMsg canMsg)) {
    cb_func = fun_ptr;
    _hasCallback = true;
}
