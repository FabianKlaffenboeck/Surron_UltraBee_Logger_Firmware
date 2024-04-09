//
// Created by FabianKlaffenboeck on 09/04/2024.
//

#ifndef SURRON_ULTRABEEFIRMWARE_ASYNCGPSHANDLER_H
#define SURRON_ULTRABEEFIRMWARE_ASYNCGPSHANDLER_H

#include <cstdlib>
#include "TinyGPS++.h"
#include <Arduino.h>

struct GPSData {
    uint32_t log;
    uint32_t lat;
    uint32_t height;
    uint32_t gpsSpeed;
    uint16_t satCnt;
};

class AsyncGpsHandler {

public:
    AsyncGpsHandler(char rxPin, char txPin);

    void loop();

    GPSData getData();

    bool hasError();

private:
    uint8_t _rxPin = 0;
    uint8_t _txPin = 0;
    uint32_t _lastUpdateT = 0;
    HardwareSerial _hws = Serial1;
    bool _hasError = false;
    TinyGPSPlus _gpsParser;
    bool _dataParser();
};


#endif //SURRON_ULTRABEEFIRMWARE_ASYNCGPSHANDLER_H
