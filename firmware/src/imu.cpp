#include "imu.h"
#include <Wire.h>
#include <SparkFun_ICM-20948_ArduinoLibrary.h>

static ICM_20948_I2C imu;
static IMUData currentData;

void imu_init(void) {
    Wire.begin();

    if (imu.begin(Wire, 0x68) != ICM_20948_Stat_Ok) {
        Serial.println("IMU init failed!");
        while (1);
    }

    Serial.println("IMU initialized.");
}

bool imu_update(IMUData *data) {
    if (data == nullptr) {
        return false;
    }

    if (imu.dataReady()) {
        imu.getAGMT();

        currentData.ax = imu.accX();
        currentData.ay = imu.accY();
        currentData.az = imu.accZ();

        currentData.gx = imu.gyrX();
        currentData.gy = imu.gyrY();
        currentData.gz = imu.gyrZ();

        currentData.mx = imu.magX();
        currentData.my = imu.magY();
        currentData.mz = imu.magZ();

        *data = currentData;
        return true;
    }

    return false;
}

void imu_print(const IMUData *data) {
    if (data == nullptr) {
        return;
    }

    Serial.print("Accel: ");
    Serial.print(data->ax); Serial.print(", ");
    Serial.print(data->ay); Serial.print(", ");
    Serial.println(data->az);

    Serial.print("Gyro: ");
    Serial.print(data->gx); Serial.print(", ");
    Serial.print(data->gy); Serial.print(", ");
    Serial.println(data->gz);

    Serial.print("Mag: ");
    Serial.print(data->mx); Serial.print(", ");
    Serial.print(data->my); Serial.print(", ");
    Serial.println(data->mz);
}
