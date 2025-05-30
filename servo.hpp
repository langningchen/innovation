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

#include <pwm.hpp>

class SERVO : public PWM
{
    uint8_t aglRng, minDuty, maxDuty;
    int16_t lastAngle = 0;

public:
    SERVO(uint8_t pin, uint32_t freq, uint8_t resolution,
          uint8_t aglRng, uint8_t minDuty, uint8_t maxDuty);
    bool setAngle(int16_t angle);
};
