#include <Arduino.h>
#include "gps.h"
#include "csvlogger.h"

GPSData gpsData;

char csvHeader = [CSV_BUFFER_SIZE];
char csvRow = [CSV_BUFFER_SIZE];


void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Initializing GPS...");
    gps_init();

    if(csvlogger_get_header(csvHeader, sizeof(csvHeader)))
    {
        Serial.println(csvHeader);
    }

    else 
    {
        Serial.println("Failed to print CSV header.");
    }
}

void loop() {
    bool gps_ok = gps_update(&gpsData)
   // bool imu_ok = imu_update(&imuData)

    if(gps_ok)
    {
        if(csvlogger_format_row(csvRow, sizeof(csvRow), &gpsData))
         {
            Serialprintln(csvRow);
         }

         else
         {
            Serialprintln("Failed to print CSV Row.");
         }
    }
    else 
    {
        Serialprintln("Senor update failed.");
    }

    
    delay(1000);
}
