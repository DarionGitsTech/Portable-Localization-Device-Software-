#ifndef GPS_H
#define GPS_H

#include <stdbool.h>

typedef struct {
    float latitude;
    float longitude;
    float altitude;
    int satellites;
    bool fix;
} GPSData;

void gps_init(void);
bool gps_update(GPSData *data);
void gps_print(const GPSData *data);

#endif
