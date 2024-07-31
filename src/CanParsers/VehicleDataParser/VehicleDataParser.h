//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#ifndef FIRMWARE_VEHICLEDATAPARSER_H
#define FIRMWARE_VEHICLEDATAPARSER_H

#include "../../Drivers/CanDriver/CanModels.h"
#include "models/VehicleData.h"

struct VehicleBusData {
    uint8_t speed = 0;
};

class VehicleDataParser {
public:
    void pars(CanMsg canMsg);

    VehicleBusData getLatestData();

private:
    VehicleBusData _latestData;
};


#endif //FIRMWARE_VEHICLEDATAPARSER_H
