#include <Arduino.h>
#include "gps.h"
#include "imu.h"
#include "csvlogger.h"
#include "ble.h"
#include "lcd.h"

GPSData gpsData;
IMUData imuData;

char csvHeader[CSV_BUFFER_SIZE];
char csvRow[CSV_BUFFER_SIZE];

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    delay(500);
    Serial.println("Initializing system...");

    gps_init();
    imu_init();
    ble_init(9600);
    lcd_init();

    if (csvlogger_get_header(csvHeader, sizeof(csvHeader))) {
        Serial.println(csvHeader);
        ble_send_line(csvHeader);
    } else {
        Serial.println("Failed to create CSV header.");
    }
}

void loop() {
    bool gps_ok = gps_update(&gpsData);
    bool imu_ok = imu_update(&imuData);

    if (gps_ok) {
        lcd_update(&gpsData);
    } else {
        lcd_show_no_fix();
    }

    if (gps_ok && imu_ok) {
        if (csvlogger_format_row(csvRow, sizeof(csvRow), &gpsData, &imuData)) {
            Serial.println(csvRow);
            ble_send_line(csvRow);
        } else {
            Serial.println("Failed to format CSV row.");
        }
    } else {
        Serial.println("Sensor update failed.");
    }

    delay(1000);
}