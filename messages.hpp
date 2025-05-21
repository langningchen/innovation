// Copyright (C) 2025 langningchen
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

/**
 * @brief Message structure for the control
 * @details This structure is used for the control to send messages to the boat
 */
struct CONTROL_MSG
{
    enum
    {
        CONTROL_NOP,
        CONTROL_CONFIG_MSG,
        CONTROL_COMMAND_MSG,
    } type;
    struct CONFIG_MSG
    {
        bool motor0Direction, motor1Direction;
        uint8_t controlTimeout;
    };
    struct COMMAND_MSG
    {
        int16_t leftServoDegree;
        int16_t rightServoDegree;
        int8_t leftMotorSpeed;
        int8_t rightMotorSpeed;
    };
    union
    {
        CONFIG_MSG configMsg;
        COMMAND_MSG commandMsg;
    };
};

/**
 * @brief Message structure for the boat
 * @details This structure is used for the boat to send messages to the control
 */
struct BOAT_MSG
{
    enum
    {
        BOAT_NOP,
        BOAT_INIT_MSG,
        BOAT_STATUS_MSG,
    } type;
    struct INIT_MSG
    {
        char compileDatetime[32];
        enum
        {
            SUCCESS,
            I2C_FAILED,
            SERVO0_FAILED,
            SERVO1_FAILED,
            MOTOR0_FAILED,
            MOTOR1_FAILED,
            BATTERY_FAILED,
            MPU6050_FAILED,
        } status;
    };
    struct STATUS_MSG
    {
        float_t batteryVoltage;
        uint8_t batteryPercentage;
        float_t mpuAX, mpuAY, mpuAZ;
        float_t mpuGX, mpuGY, mpuGZ;
    };
    union
    {
        INIT_MSG initMsg;
        STATUS_MSG statusMsg;
    };
};
