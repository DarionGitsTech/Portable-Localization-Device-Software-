#include <Arduino.h>
#include "gps.h"

GPSData gpsData;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Initializing GPS...");
    gps_init();
}

void loop() {
    if (gps_update(&gpsData)) {
        gps_print(&gpsData);
        Serial.println("------------------------");
    } else {
        Serial.println("GPS update failed.");
    }

    delay(1000);
}
