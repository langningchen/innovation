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

#include <pin.hpp>
#define MPU_ADDRESS 0x68
#define OLED_ADDRESS 0x3C
#define INA_ADDRESS 0x40
#define OLED_WIDTH 128      // px
#define OLED_HEIGHT 64      // px
#define MSG_INTERVAL 50     // ms
#define SERVO_RANGE 135     // d
#define OLED_LINE_CNT 8     // lines
#define OLED_CHAR_WIDTH 6   // px
#define OLED_CHAR_HEIGHT 12 // px
#define GRAVITY 9.81f       // m/s^2

enum DIR
{
    NONE,
    UP,
    DOWN,
    RIGHT,
    LEFT,
};
