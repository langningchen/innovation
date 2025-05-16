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

#include <define.hpp>
#include <functional>

class A2D
{
private:
    const uint8_t ADC_RESOLUTION = 12; // ADC resolution in bits
    const float_t configFilterCoef = 0.5;
    const float_t speedCruiseFilterCoef = 0.5;
    const float_t speedControlFilterCoef = 0.5;
    const float_t steerControlFilterCoef = 0.5;

    uint16_t configMax, speedCruiseMax, speedControlMax, steerControlMax;
    uint16_t speedControlBasis, steerControlBasis;
    uint16_t configMin, speedCruiseMin, speedControlMin, steerControlMin;

    uint8_t configPin, speedCruisePin;
    uint8_t speedControlPin, steerControlPin;
    uint8_t enableCruisePin, enableLockPin;

    uint16_t config, speedCruise;
    uint16_t speedControl, steerControl;
    bool enableCruise, enableLock;

    DIR lastDirection = NONE;
    std::function<void(DIR)> onDirectionEnd = nullptr;

    DIR getDirection();
    void updateDirection();

public:
    A2D(uint8_t configPin, uint8_t speedCruisePin,
        uint8_t speedControlPin, uint8_t steerControlPin,
        uint8_t enableCruisePin, uint8_t enableLockPin);
    void begin();
    void reset();
    void setBasis();
    void process();
    void getData(uint8_t &config, uint8_t &speedCruise,
                 int8_t &speedControl, int8_t &steerControl,
                 bool &enableCruise, bool &enableLock);
    void updateMinMax();
    void setOnDirectionEnd(std::function<void(DIR)> callback);
};
