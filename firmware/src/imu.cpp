#include "imu.h"
#include <Arduino.h>
#include <Wire.h>

static IMUData currentData;

bool imu_init(void) {
    Wire.begin();
    
    // Placeholder initialization
    currentData.valid = false;
    
    return true;
}

bool imu_update(IMUData *data) {
    if (data == nullptr) {
        return false;
    }
    
    // --- Placeholder values (replace later with real sensor reads) ---
    currentData.accel_x = 0.01;
    currentData.accel_y = -0.02;
    currentData.accel_z = 9.81;
    
    currentData.gyro_x = 0.1;
    currentData.gyro_y = 0.2;
    currentData.gyro_z = 0.3;
    
    currentData.mag_x = 30.0;
    currentData.mag_y = -15.0;
    currentData.mag_z = 45.0;
    
    currentData.valid = true;
    
    *data = currentData;
    return true;
}

void imu_print(const IMUData *data) {
    if (data == nullptr) return;
    
    Serial.println("IMU Data:");
    
    Serial.print("Accel (m/s^2): ");
    Serial.print(data->accel_x); Serial.print(", ");
    Serial.print(data->accel_y); Serial.print(", ");
    Serial.println(data->accel_z);
    
    Serial.print("Gyro (rad/s): ");
    Serial.print(data->gyro_x); Serial.print(", ");
    Serial.print(data->gyro_y); Serial.print(", ");
    Serial.println(data->gyro_z);
    
    Serial.print("Mag (uT): ");
    Serial.print(data->mag_x); Serial.print(", ");
    Serial.print(data->mag_y); Serial.print(", ");
    Serial.println(data->mag_z);
    
    Serial.println("------------------------");
}
