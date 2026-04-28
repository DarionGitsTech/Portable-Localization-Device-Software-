#include <Arduino.h>
#include <SD.h>
#include "gps.h"
#include "imu.h"
#include "lcd.h"
#include "csvlogger.h"
#include "ble.h"

// Global sensor data
GPSData gpsData;
IMUData imuData;

char csvHeader[CSV_BUFFER_SIZE];
char csvRow[CSV_BUFFER_SIZE];

File logFile;

void setup() {
    Serial.begin(115200);
    while (!Serial) { }
    delay(500);

    Serial.println("Starting full integration test with CSV...");

    gps_init();
    imu_init();
    lcd_init();
    ble_init(9600);

    // Print CSV header once
    if (csvlogger_get_header(csvHeader, sizeof(csvHeader))) {
        Serial.println(csvHeader);
    } else {
        Serial.println("Failed to create CSV header.");
    }

    // Initialize SD card
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SD initialization failed!");
    } else {
        Serial.println("SD initialization success.");

        // Open file
        SD.remove("log.csv");
        logFile = SD.open("log.csv", FILE_WRITE);

        if (logFile) {
            Serial.println("Opened log.csv");

            // Write header once
            if (csvlogger_get_header(csvHeader, sizeof(csvHeader))) {
                logFile.println(csvHeader);
                logFile.flush();   // ensure it's written immediately
            }
        } else {
            Serial.println("Failed to open log.csv");
        }
    }
}

void loop() {
    static unsigned long lastPrint = 0;
    static unsigned long lastImuUpdate = 0;
    static unsigned long lastLcdUpdate = 0;

    // GPS is highest priority: run every loop
    bool gps_ok = gps_update(&gpsData);

    // IMU throttled to 20 Hz
    bool imu_ok = true;
    if (millis() - lastImuUpdate >= 50) {
        lastImuUpdate = millis();
        imu_ok = imu_update(&imuData);
    }

    // LCD throttled to reduce I2C traffic
    if (millis() - lastLcdUpdate >= 300) {
        lastLcdUpdate = millis();

        if (gps_ok) {
            lcd_update(&gpsData);
        } else {
            lcd_show_no_fix();
        }
    }

    // Print CSV once per second
    if (millis() - lastPrint >= 1000) {
        lastPrint = millis();

        // Helpful debug line
        Serial.print("GPS fix=");
        Serial.print(gpsData.fix);
        Serial.print(" sats=");
        Serial.print(gpsData.satellites);
        Serial.print(" lat=");
        Serial.print(gpsData.latitude, 6);
        Serial.print(" lon=");
        Serial.print(gpsData.longitude, 6);
        Serial.print(" | IMU ok=");
        Serial.println(imu_ok ? "YES" : "NO");

        if (gps_ok && imu_ok) {
            if (csvlogger_format_row(csvRow, sizeof(csvRow), &gpsData, &imuData)) {
                Serial.println(csvRow);
                ble_send_line(csvRow); // added ble_send_line here

                // Write to SD card
                if (logFile) {
                    logFile.println(csvRow);
                    logFile.flush();
                }
            } else {
                Serial.println("Failed to format CSV row.");
            }
        } else {
            Serial.println("Sensor update failed.");
        }
    }
    while (ble_available()) {
        int c = ble_read_char();
        if (c >= 0) {
            Serial.print((char)c);
        }
    } // end while (for text from phone to Serial Monitor)
}
