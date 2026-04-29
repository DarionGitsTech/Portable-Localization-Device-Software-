#ifndef GPS_H
#define GPS_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    float latitude;
    float longitude;
    float altitude;
    int satellites;
    bool fix;

    // Date and Time
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
} GPSData;

void gps_init(void);
bool gps_update(GPSData *data);
void gps_print(const GPSData *data);

#endif
