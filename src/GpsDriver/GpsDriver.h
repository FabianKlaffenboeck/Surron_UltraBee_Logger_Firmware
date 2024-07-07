//
// Created by FabianKlaffenboeck on 07/07/2024.
//

#ifndef FIRMWARE_GPSDRIVER_H
#define FIRMWARE_GPSDRIVER_H

#include <TinyGPS++.h>
#include <cstdlib>

struct GpsData {
    uint32_t date;
    uint32_t time;
    double lat;
    double lng;
    double altitude;
    uint8_t sat_cnt;
};

class GpsDriver {

public:
    GpsDriver(uint8_t rxPin, uint8_t txPin);

    void gpsHandlerLoop();

    bool hasSignal() const;

    GpsData getLocData();


private:
    uint8_t _rxPin;
    uint8_t _txPin;
    HardwareSerial *_hws;

    TinyGPSPlus *_gpsParser;

    GpsData _gpsData;
    bool _hasSignal;

    bool _parsData(char gpsData);
};


#endif //FIRMWARE_GPSDRIVER_H
