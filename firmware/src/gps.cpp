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
        return false;
    }

    char c;

    // Read all incoming GPS characters
    while ((c = GPS.read())) {
        (void)c;
    }

    // When a full sentence arrives
    while (GPS.newNMEAreceived()) {
        const char* sentence = GPS.lastNMEA();

        // Only print GGA and RMC sentences
        if (strstr(sentence, "GGA") || strstr(sentence, "RMC")) {
            Serial.print("RAW: ");
            Serial.println(sentence);
        }

        if (GPS.parse(sentence)) {

            // Print parsed status clearly
            Serial.print("PARSED -> fix=");
            Serial.print(GPS.fix);
            Serial.print(" sats=");
            Serial.print(GPS.satellites);

            if (GPS.fix) {
                Serial.print(" lat=");
                Serial.print(GPS.latitudeDegrees, 6);
                Serial.print(" lon=");
                Serial.print(GPS.longitudeDegrees, 6);
                Serial.print(" alt=");
                Serial.print(GPS.altitude);
            }

            Serial.println();
        } else {
            Serial.println("PARSE FAILED");
        }
    }

    // Update struct (same as before)
    currentData.fix = GPS.fix;
    currentData.satellites = GPS.satellites;

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
