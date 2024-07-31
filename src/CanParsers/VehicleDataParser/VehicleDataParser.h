//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#ifndef FIRMWARE_VEHICLEDATAPARSER_H
#define FIRMWARE_VEHICLEDATAPARSER_H

#include "../../Drivers/CanDriver/CanModels.h"
#include "models/VehicleData.h"

struct VehicleBusData{
    uint8_t speed = 0;
};

class VehicleDataParser {
public:
    VehicleBusData pars(CanMsg canMsg);
};


#endif //FIRMWARE_VEHICLEDATAPARSER_H
