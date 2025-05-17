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
        uint8_t controlTimeout; // 0 ~ 3000 ms
        uint8_t maxSpeed;       // 0 ~ 100 %
        int8_t leftServoDelta;  // -30 ~ 30 d
        int8_t rightServoDelta; // -30 ~ 30 d
    } data;
    const DATA defaultData = {
        .controlTimeout = 10, // 100
        .maxSpeed = 100,      // 100
        .leftServoDelta = 0,  // 0
        .rightServoDelta = 0, // 0
    };

public:
    bool begin();
    bool save();
    uint8_t getControlTimeout();
    bool setControlTimeout(uint8_t controlTimeout);
    uint8_t getMaxSpeed();
    bool setMaxSpeed(uint8_t maxSpeed);
    int8_t getLeftServoDelta();
    bool setLeftServoDelta(int8_t leftServoDelta);
    int8_t getRightServoDelta();
    bool setRightServoDelta(int8_t rightServoDelta);
};
