// Copyright (C) 2025 Langning Chen
//
// This file is part of innovation.
//
// innovation is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// innovation is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with innovation.  If not, see <https://www.gnu.org/licenses/>.

// Copyright (C) 2025 Langning Chen
//
// This file is part of innovation.
//
// innovation is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// innovation is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with innovation.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <Adafruit_MPU6050.h>

class MPU6050
{
private:
    Adafruit_MPU6050 mpu;
    uint8_t pin;
    uint8_t address;
    mpu6050_accel_range_t accelRange;
    mpu6050_gyro_range_t gyroRange;

public:
    MPU6050(uint8_t pin, uint8_t address,
            mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange);
    bool begin();
    void readData(float_t &ax, float_t &ay, float_t &az,
                  float_t &gx, float_t &gy, float_t &gz);
};
