//
// Created by FabianKlaffenboeck on 31/07/2024.
//

#ifndef FIRMWARE_VEHICLEDATA_H
#define FIRMWARE_VEHICLEDATA_H

struct VehicleData {
    uint8_t vehicleSpeed = 0;
    uint32_t time = 0;
    double lat = 0;
    double lng = 0;
    double altitude = 0;
};

#endif //FIRMWARE_VEHICLEDATA_H
