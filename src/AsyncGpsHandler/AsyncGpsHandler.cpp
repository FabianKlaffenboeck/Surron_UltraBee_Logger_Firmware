//
// Created by FabianKlaffenboeck on 09/04/2024.
//

#include "AsyncGpsHandler.h"

AsyncGpsHandler::AsyncGpsHandler(char rxPin, char txPin) {
    _hws.begin(9600, SERIAL_8N1, rxPin, txPin);
}

void AsyncGpsHandler::loop() {
    while (_hws.available()) {
        char gpsData = _hws.read();
        _gpsParser.encode(gpsData);
    }

    if (_gpsParser.location.isUpdated()) {
        _dataParser();
    }

    _hasError = _gpsParser.failedChecksum() || !_gpsParser.satellites.isValid() || !_gpsParser.satellites.isValid();
}

GpsData AsyncGpsHandler::getData() {
    return _gpsData;
}

bool AsyncGpsHandler::hasError() {
    return _hasError;
}

bool AsyncGpsHandler::_dataParser() {
    _gpsData = {
            _gpsParser.date.value(),
            _gpsParser.time.value(),
            _gpsParser.location.lat(),
            _gpsParser.location.lng(),
            _gpsParser.altitude.meters(),
            (uint8_t) _gpsParser.satellites.value()
    };
    return true;
}
