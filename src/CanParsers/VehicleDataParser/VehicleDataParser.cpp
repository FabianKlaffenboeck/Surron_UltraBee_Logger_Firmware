//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#include "VehicleDataParser.h"
#include "models/VehicleData.h"

void VehicleDataParser::pars(CanMsg canMsg) {
    switch (canMsg.id) {
        case 0x221:
            _latestData.speed = canMsg.data[0];
        default:
            return;
    }
}

VehicleBusData VehicleDataParser::getLatestData() {
    return _latestData;
}
