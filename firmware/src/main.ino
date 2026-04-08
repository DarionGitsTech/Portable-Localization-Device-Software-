#include <Arduino.h>
#include "gps.h"
#include "imu.h"
#include "csvlogger.h"
#include "ble.h"
#include "lcd.h"

GPSData gpsData;
IMUData imuData;

char csvHeader = [CSV_BUFFER_SIZE];
char csvRow = [CSV_BUFFER_SIZE];


void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Initializing system...");

    gps_init();
    imu_init();
    ble_init(9600); // HM-10 usually defaults to 9600 baud
    lcd_init();

    if(csvlogger_get_header(csvHeader, sizeof(csvHeader)))
    {
        Serial.println(csvHeader);
        ble_send_line(csvHeader);
    }

    else 
    {
        Serial.println("Failed to create the CSV header.");
    }
} // end void setup()



void loop() {
    bool gps_ok = gps_update(&gpsData)
    bool imu_ok = imu_update(&imuData)

    if (gps_ok) 
    {
        lcd_update(&gpsData);
    }

    if(gps_ok && imu_ok)
    {
        if(csvlogger_format_row(csvRow, sizeof(csvRow), millis(), &gpsData, &imudata))
         {
            Serialprintln(csvRow);
            ble_send_line(csvRow);
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
} // end void loop()
