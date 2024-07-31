//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#include "VehicleDataParser.h"
#include "models/VehicleData.h"

void VehicleDataParser::pars(CanMsg canMsg) {

}

VehicleBusData VehicleDataParser::getLatestData() {
    return _latestData;
}
