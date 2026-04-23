#include "gps.h"
#include <Wire.h>
#include <Adafruit_GPS.h>
// tester stripped-down code for GPS debugging

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
    Serial.println("gps_init complete");
}

bool gps_update(GPSData *data) {
    if (data == nullptr || !gps_initialized) {
        Serial.println("gps_update: init/data failure");
        return false;
    }

    int charsRead = 0;
    char c;

    // Drain all available GPS chars
    while ((c = GPS.read())) {
        charsRead++;
    }

    if (charsRead > 0) {
        Serial.print("gps_update: charsRead=");
        Serial.println(charsRead);
    }

    // Check for complete NMEA sentences and parse them
    while (GPS.newNMEAreceived()) {
        Serial.print("gps_update: sentence=");
        Serial.println(GPS.lastNMEA());

        if (GPS.parse(GPS.lastNMEA())) {
            Serial.println("gps_update: parse OK");
        } else {
            Serial.println("gps_update: parse FAILED");
        }
    }

    currentData.fix = GPS.fix;
    currentData.satellites = GPS.satellites;

    if (GPS.fix) {
        currentData.latitude = GPS.latitudeDegrees;
        currentData.longitude = GPS.longitudeDegrees;
        currentData.altitude = GPS.altitude;
    }

    Serial.print("gps_update: fix=");
    Serial.print(currentData.fix);
    Serial.print(" sats=");
    Serial.print(currentData.satellites);
    Serial.print(" lat=");
    Serial.print(currentData.latitude, 6);
    Serial.print(" lon=");
    Serial.println(currentData.longitude, 6);

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
