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

class MOTOR : public PWM
{
private:
    uint8_t dirPin; // Direction pin
    bool dir;       // Direction

public:
    MOTOR(uint8_t pin, uint8_t dirPin, uint32_t freq, uint8_t channel, bool dir);
    void setDirection(bool dir);
    bool setSpeed(int8_t speed);
};
