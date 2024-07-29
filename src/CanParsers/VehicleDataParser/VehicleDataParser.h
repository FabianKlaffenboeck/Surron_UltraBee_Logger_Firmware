//
// Created by FabianKlaffenboeck on 29/07/2024.
//

#ifndef FIRMWARE_VEHICLEDATAPARSER_H
#define FIRMWARE_VEHICLEDATAPARSER_H

#include "../../Drivers/CanDriver/CanModels.h"


class VehicleDataParser {
public:
    void pars(CanMsg canMsg);
};


#endif //FIRMWARE_VEHICLEDATAPARSER_H
