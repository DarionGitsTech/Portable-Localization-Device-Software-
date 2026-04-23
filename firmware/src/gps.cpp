#include "gps.h"
#include <Wire.h>
#include <Adafruit_GPS.h>


static Adafruit_GPS GPS(&Wire);
static bool gps_initialized = false;
static GPSData currentData;


void gps_init(void) {
    Wire.begin();


    GPS.begin(0x10); // PA1010D I2C address


    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    GPS.sendCommand(PGCMD_ANTENNA);


    currentData.latitude = 0.0f;
    currentData.longitude = 0.0f;
    currentData.altitude = 0.0f;
    currentData.satellites = 0;
    currentData.fix = false;


    gps_initialized = true;
}


bool gps_update(GPSData *data) {
    if (data == nullptr || !gps_initialized) {
        return false;
    }


    // Drain all available characters from the GPS
    char c;
    while ((c = GPS.read())) {
        (void)c;
    }


    // Parse every complete sentence currently buffered
    while (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA())) {
            break;
        }
    }


    // Always update fix/satellite status
    currentData.fix = GPS.fix;
    currentData.satellites = GPS.satellites;


    // Only overwrite coordinates when we have a valid fix
    if (GPS.fix) {
        currentData.latitude = GPS.latitudeDegrees;
        currentData.longitude = GPS.longitudeDegrees;
        currentData.altitude = GPS.altitude;
    }


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
    Serial.println(data->altitude, 2);


    Serial.print("Satellites: ");
    Serial.println(data->satellites);
}
