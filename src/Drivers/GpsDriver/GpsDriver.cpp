//
// Created by FabianKlaffenboeck on 07/07/2024.
//

#include "GpsDriver.h"

GpsDriver::GpsDriver(uint8_t rxPin, uint8_t txPin) {

    _rxPin = rxPin;
    _txPin = txPin;

    _gpsParser = new TinyGPSPlus();

    _gpsData = {};

    _hws = &Serial1;
    _hws->begin(9600, SERIAL_8N1, _rxPin, _txPin);
}

void GpsDriver::loop() {

    while (_hws->available() > 0) {
        char c = _hws->read();
        _parsData(c);
    }
}

GpsData GpsDriver::getLocData() {
    return _gpsData;
}


bool GpsDriver::_parsData(char gpsData) {

    _gpsParser->encode(gpsData);

    if (!_gpsParser->location.isValid())
    {
        return false;
    }

    _gpsData = {
            _gpsParser->date.value(),
            _gpsParser->time.value(),
            _gpsParser->location.lat(),
            _gpsParser->location.lng(),
            _gpsParser->altitude.meters(),
            (uint8_t) _gpsParser->satellites.value()
    };

    return true;
}

