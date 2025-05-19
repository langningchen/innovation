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

#include <EEPROM.h>

class STORAGE
{
private:
    struct DATA
    {
        uint8_t controlTimeout;                 // 0 ~ 3000 ms
        uint8_t maxSpeed;                       // 0 ~ 100 %
        int8_t leftServoDelta, rightServoDelta; // -30 ~ 30 d
        int8_t leftMotorDelta, rightMotorDelta; // -30 ~ 30 %
        uint8_t backwardLimit;                  // 0 ~ 100 %
        uint8_t dirThreshold;                   // 20 ~ 90 %
        uint8_t servoLimit;                     // 30 ~ 100 d
        uint16_t networkThreshold;              // 1000 ~ 10000 ms
        uint8_t batteryThreshold;               // 30 ~ 70 %
        float_t mpuXThreshold;                  // 1 ~ 7
        float_t mpuYThreshold;                  // 1 ~ 7
        float_t mpuZThreshold;                  // 7 ~ 10
    } data;
    const DATA defaultData = {
        .controlTimeout = 10,
        .maxSpeed = 100,
        .leftServoDelta = 0,
        .rightServoDelta = 0,
        .leftMotorDelta = 0,
        .rightMotorDelta = 0,
        .backwardLimit = 30,
        .dirThreshold = 30,
        .servoLimit = 60,
        .networkThreshold = 5000,
        .batteryThreshold = 50,
        .mpuXThreshold = 3.0,
        .mpuYThreshold = 3.0,
        .mpuZThreshold = 9.5,
    };

public:
    bool begin();
    bool save();
    bool reset();

    uint8_t getControlTimeout();
    bool setControlTimeout(uint8_t controlTimeout);

    uint8_t getMaxSpeed();
    bool setMaxSpeed(uint8_t maxSpeed);

    int8_t getLeftServoDelta();
    bool setLeftServoDelta(int8_t leftServoDelta);

    int8_t getRightServoDelta();
    bool setRightServoDelta(int8_t rightServoDelta);

    int8_t getLeftMotorDelta();
    bool setLeftMotorDelta(int8_t leftMotorDelta);

    int8_t getRightMotorDelta();
    bool setRightMotorDelta(int8_t rightMotorDelta);

    uint8_t getBackwardLimit();
    bool setBackwardLimit(uint8_t backwardLimit);

    uint8_t getDirThreshold();
    bool setDirThreshold(uint8_t dirThreshold);

    uint8_t getServoLimit();
    bool setServoLimit(uint8_t servoLimit);

    uint16_t getNetworkThreshold();
    bool setNetworkThreshold(uint16_t networkThreshold);

    uint8_t getBatteryThreshold();
    bool setBatteryThreshold(uint8_t batteryThreshold);

    float_t getMpuXThreshold();
    bool setMpuXThreshold(float_t mpuXThreshold);

    float_t getMpuYThreshold();
    bool setMpuYThreshold(float_t mpuYThreshold);

    float_t getMpuZThreshold();
    bool setMpuZThreshold(float_t mpuZThreshold);
};
