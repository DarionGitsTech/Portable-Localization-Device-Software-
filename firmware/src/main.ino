#include <Arduino.h>
#include "gps.h"
// isolate GPS data for .ino (debugging)
GPSData gpsData;

void setup() {
    Serial.begin(115200);
    while (!Serial) { }
    delay(500);

    Serial.println("Starting GPS-only integration debug...");
    gps_init();
}

void loop() {
    gps_update(&gpsData);
    delay(200);
}
