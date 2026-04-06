#ifndef IMU_H
#define IMU_H

#include <stdbool.h>

typedef struct {
  float accel_x;
  float accel_y;
  float accel_z;
  
  float gyro_x;
  float gyro_y;
  float gyro_z;
  
  float mag_x;
  float mag_y;
  float mag_z;
  
  bool valid;
} IMUData;

// Initialize IMU
bool imu_init(void);

// Update IMU readings
bool imu_update(IMUData *data);

// Print IMU data (for debugging)
void imu_print(const IMUData *data);

#endif