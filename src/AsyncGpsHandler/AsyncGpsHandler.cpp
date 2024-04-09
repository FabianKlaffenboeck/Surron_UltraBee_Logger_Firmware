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
        Serial.write(gpsData);
        _gpsParser.encode(gpsData);
        // _gpsParser.encode(_hws.read());
    }

    if (_gpsParser.location.isUpdated()) {
        _dataParser();
    }

    _hasError = _gpsParser.failedChecksum() || !_gpsParser.satellites.isValid() || !_gpsParser.satellites.isValid();
}

GpsData AsyncGpsHandler::getData() {
    GpsData gpsData = {0, 0, 0, 0, 0};
    return gpsData;
}

bool AsyncGpsHandler::hasError() {
    return _hasError;
}

bool AsyncGpsHandler::_dataParser() {
    Serial.print("Satellites=");
    Serial.println(_gpsParser.satellites.value());
    Serial.print("Latitude=");
    Serial.print(_gpsParser.location.lat(), 6);
    Serial.print("Longitude=");
    Serial.println(_gpsParser.location.lng(), 6);
    return true;
}
