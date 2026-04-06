#ifndef CSVLOGGER_H
#define CSVLOGGER_H

#include <stddef.h>
#include <stdbool.h>
#include "gps.h"
#include "imu.h"

#define CSV_BUFFER_SIZE 256

// Writes the CSV header into the provided buffer
bool csvlogger_get_header(char *buffer, size_t buffer_size);

// Writes one CSV data row into the provided buffer
bool csvlogger_format_row(char *buffer,
                          size_t buffer_size,
                          const GPSData *gps,
                          const IMUData *imu);

#endif
