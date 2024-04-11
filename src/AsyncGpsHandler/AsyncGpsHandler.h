//
// Created by FabianKlaffenboeck on 09/04/2024.
//

#ifndef SURRON_ULTRABEEFIRMWARE_ASYNCGPSHANDLER_H
#define SURRON_ULTRABEEFIRMWARE_ASYNCGPSHANDLER_H

#include <cstdlib>
#include "TinyGPS++.h"
#include <Arduino.h>

struct GpsData {
    uint32_t date;
    uint32_t time;
    double lat;
    double lng;
    double altitude;
    uint8_t sat_cnt;
};

class AsyncGpsHandler {

public:
    AsyncGpsHandler(char rxPin, char txPin);

    void loop();

    GpsData getData();

    bool hasError();

private:
    uint8_t _rxPin = 0;
    uint8_t _txPin = 0;
    HardwareSerial _hws = Serial1;
    //uint32_t _lastUpdateT = 0;
    GpsData _gpsData;
    bool _hasError = false;
    TinyGPSPlus _gpsParser;

    bool _dataParser();
};


#endif //SURRON_ULTRABEEFIRMWARE_ASYNCGPSHANDLER_H
