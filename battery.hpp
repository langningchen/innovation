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

#include <INA226.h>

class BATTERY
{
private:
    uint8_t address;
    float_t minVoltage;
    float_t maxVoltage;
    INA226 *ina226;

public:
    BATTERY(uint8_t address, float_t minVoltage, float_t maxVoltage);
    bool begin();
    float_t getVoltage();
    uint8_t getPercentage();
};
