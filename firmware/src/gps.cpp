#include "gps.h"
#include <Wire.h>

// Placeholder global data until library calls are added
static GPSData currentData;

void gps_init(void) {
    Wire.begin();

    currentData.latitude = 0.0;
    currentData.longitude = 0.0;
    currentData.altitude = 0.0;
    currentData.satellites = 0;
    currentData.fix = false;
}

bool gps_update(GPSData *data) {
    if (data == nullptr) {
        return false;
    }

    // Placeholder values for early testing
    // Replace with real GPS reads once library/device comms are added
    currentData.latitude = 35.0844;
    currentData.longitude = -106.6504;
    currentData.altitude = 1619.0;
    currentData.satellites = 6;
    currentData.fix = true;

    *data = currentData;
    return true;
}

void gps_print(const GPSData *data) {
    if (data == nullptr) {
        return;
    }

    Serial.print("Fix: ");
    Serial.println(data->fix ? "YES" : "NO");

    Serial.print("Latitude: ");
    Serial.println(data->latitude, 6);

    Serial.print("Longitude: ");
    Serial.println(data->longitude, 6);

    Serial.print("Altitude: ");
    Serial.println(data->altitude);

    Serial.print("Satellites: ");
    Serial.println(data->satellites);
}