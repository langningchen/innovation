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
    int16_t leftServoDegree;
    int16_t rightServoDegree;
    int8_t leftMotorSpeed;
    int8_t rightMotorSpeed;
};

/**
 * @brief Message structure for the boat
 * @details This structure is used for the boat to send messages to the control
 */
struct BOAT_MSG
{
    uint8_t result;
    float_t batteryVoltage;
    uint8_t batteryPercentage;
    float_t mpuX, mpuY, mpuZ;
};
