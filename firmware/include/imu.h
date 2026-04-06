#ifndef IMU_H
#define IMU_H

#include <stdbool.h>

typedef struct {
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
} IMUData;

void imu_init(void);
bool imu_update(IMUData *data);
void imu_print(const IMUData *data);

#endif
