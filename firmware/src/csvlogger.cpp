#include "csvlogger.h"
#include <Arduino.h>
#include <stdio.h>

bool csvlogger_get_header(char *buffer, size_t buffer_size) {
    if (buffer == nullptr || buffer_size == 0) {
        return false;
    }

    int written = snprintf(
        buffer,
        buffer_size,
        "date,time,latitude,longitude,altitude,satellites,fix,"
        "accel_x,accel_y,accel_z,"
        "gyro_x,gyro_y,gyro_z,"
        "mag_x,mag_y,mag_z"
    );

    return (written > 0 && (size_t)written < buffer_size);
}

bool csvlogger_format_row(char *buffer,
                          size_t buffer_size,
                          const GPSData *gps,
                          const IMUData *imu) {
    if (buffer == nullptr || buffer_size == 0 || gps == nullptr || imu == nullptr) {
        return false;
    }

    char dateStr[16];
    char timeStr[16];

    // Format date: DD/MM/YYYY
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", gps->day, gps->month, gps->year);

    // Format time: HH:MM:SS
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", gps->hour, gps->minute, gps->seconds);

    if (!gps->fix) {
        snprintf(dateStr, sizeof(dateStr), "0000/00/00");
        snprintf(timeStr, sizeof(timeStr), "00:00:00");
    }

    int written = snprintf(
        buffer,
        buffer_size,
        "%s,%s,%.6f,%.6f,%.2f,%d,%d,"
        "%.3f,%.3f,%.3f,"
        "%.3f,%.3f,%.3f,"
        "%.3f,%.3f,%.3f",
        dateStr,
        timeStr,
        gps->latitude,
        gps->longitude,
        gps->altitude,
        gps->satellites,
        gps->fix ? 1 : 0,
        imu->accel_x,
        imu->accel_y,
        imu->accel_z,
        imu->gyro_x,
        imu->gyro_y,
        imu->gyro_z,
        imu->mag_x,
        imu->mag_y,
        imu->mag_z
    );

    return (written > 0 && (size_t)written < buffer_size);
}
