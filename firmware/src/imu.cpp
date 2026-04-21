#include "imu.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <math.h>

static LSM9DS1 imu;

// Unit conversion constants
static constexpr float G_TO_MS2 = 9.80665f;               // g -> m/s^2
static constexpr float DEG2RAD = 0.0174532925199433f;  // deg/s -> rad/s
static constexpr float GAUSS_TO_UT = 100.0f;              // gauss -> microtesla

// LSM9DS1 default I2C addresses for your board
static constexpr uint8_t LSM9DS1_AG_ADDR = 0x6B; // accel/gyro
static constexpr uint8_t LSM9DS1_M_ADDR  = 0x1E; // magnetometer

static bool imu_initialized = false;
static IMUData currentData;

bool imu_init(void) {
    Wire.begin();
    Wire.setClock(100000);  // conservative bring-up speed

    imu.settings.device.commInterface = IMU_MODE_I2C;
    imu.settings.device.agAddress = LSM9DS1_AG_ADDR;
    imu.settings.device.mAddress = LSM9DS1_M_ADDR;

    // Optional but sensible defaults
    imu.settings.gyro.enabled = true;
    imu.settings.accel.enabled = true;
    imu.settings.mag.enabled = true;
    imu.settings.temp.enabled = false;

    // These library settings are in the sensor's native units:
    // gyro scale in dps, accel scale in g, mag scale in gauss.
    imu.settings.gyro.scale = 245;
    imu.settings.accel.scale = 2;
    imu.settings.mag.scale = 4;

    if (!imu.begin()) {
        imu_initialized = false;
        currentData.valid = false;
        return false;
    }

    imu_initialized = true;
    currentData.valid = false;
    return true;
}

bool imu_update(IMUData *data) {
    if (data == nullptr || !imu_initialized) {
        return false;
    }

    bool got_any_data = false;

    if (imu.accelAvailable()) {
        imu.readAccel();
        currentData.accel_x = imu.calcAccel(imu.ax) * G_TO_MS2;
        currentData.accel_y = imu.calcAccel(imu.ay) * G_TO_MS2;
        currentData.accel_z = imu.calcAccel(imu.az) * G_TO_MS2;
        got_any_data = true;
    }

    if (imu.gyroAvailable()) {
        imu.readGyro();
        currentData.gyro_x = imu.calcGyro(imu.gx) * DEG2RAD;
        currentData.gyro_y = imu.calcGyro(imu.gy) * DEG2RAD;
        currentData.gyro_z = imu.calcGyro(imu.gz) * DEG2RAD;
        got_any_data = true;
    }

    if (imu.magAvailable()) {
        imu.readMag();
        currentData.mag_x = imu.calcMag(imu.mx) * GAUSS_TO_UT;
        currentData.mag_y = imu.calcMag(imu.my) * GAUSS_TO_UT;
        currentData.mag_z = imu.calcMag(imu.mz) * GAUSS_TO_UT;
        got_any_data = true;
    }

    currentData.valid = got_any_data;

    if (!got_any_data) {
        return false;
    }

    *data = currentData;
    return true;
}

void imu_print(const IMUData *data) {
    if (data == nullptr) {
        return;
    }

    Serial.println("IMU Data:");

    Serial.print("Accel (m/s^2): ");
    Serial.print(data->accel_x, 3); Serial.print(", ");
    Serial.print(data->accel_y, 3); Serial.print(", ");
    Serial.println(data->accel_z, 3);

    Serial.print("Gyro (rad/s): ");
    Serial.print(data->gyro_x, 3); Serial.print(", ");
    Serial.print(data->gyro_y, 3); Serial.print(", ");
    Serial.println(data->gyro_z, 3);

    Serial.print("Mag (uT): ");
    Serial.print(data->mag_x, 3); Serial.print(", ");
    Serial.print(data->mag_y, 3); Serial.print(", ");
    Serial.println(data->mag_z, 3);

    Serial.println("------------------------");
}
