#include <pwm6050.hpp>

/**
 * @brief MPU6050 constructor
 * @param pin GPIO pin
 * @param address I2C address of the MPU6050
 * @param accelRange Accelerometer range
 * @param gyroRange Gyroscope range
 */
MPU6050::MPU6050(uint8_t pin, uint8_t address,
                 mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange)
    : pin(pin), address(address), accelRange(accelRange), gyroRange(gyroRange) {}

/**
 * @brief Initialize the MPU6050
 * @return true if successful, false otherwise
 */
bool MPU6050::begin()
{
    if (!mpu.begin(address))
        return false;
    mpu.setAccelerometerRange(accelRange);
    mpu.setGyroRange(gyroRange);
    return true;
}

/**
 * @brief Read data from the MPU6050
 * @param ax Accelerometer X-axis value, in m/s^2
 * @param ay Accelerometer Y-axis value, in m/s^2
 * @param az Accelerometer Z-axis value, in m/s^2
 * @param gx Gyroscope X-axis value, in rad/s
 * @param gy Gyroscope Y-axis value, in rad/s
 * @param gz Gyroscope Z-axis value, in rad/s
 */
void MPU6050::readData(float_t &ax, float_t &ay, float_t &az,
                       float_t &gx, float_t &gy, float_t &gz)
{
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    ax = a.acceleration.x, ay = a.acceleration.y, az = a.acceleration.z;
    gx = g.gyro.x, gy = g.gyro.y, gz = g.gyro.z;
}
