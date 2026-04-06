#include <Arduino.h>
#include "gps.h"
#include "imu.h"

GPSData gpsData;
IMUData imuData;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Initializing GPS...");
    
    gps_init();
    imu_init();
}

void loop() {
    Serial.println("----- NEW SAMPLE -----");

    // GPS
    if (gps_update(&gpsData)) {
        gps_print(&gpsData);
        Serial.println("------------------------");
    } else {
        Serial.println("GPS update failed.");
    }

    // IMU
    if (imu_update(&imuData)) {
        imu_print(&imuData);
        Serial.println("------------------------");
    } else {
        Serial.println("IMU update failed.");
    }

    delay(1000);
}
