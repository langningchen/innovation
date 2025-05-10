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

class A2D
{
private:
    const uint8_t ADC_RESOLUTION = 12; // ADC resolution in bits
    const float_t speedMaxFilterCoef = 0.08;
    const float_t speedCruiseFilterCoef = 0.05;
    const float_t speedControlFilterCoef = 0.05;
    const float_t steerControlFilterCoef = 0.08;

    uint8_t speedControlBasis, steerControlBasis;

    uint8_t speedMaxPin, speedCruisePin;
    uint8_t speedControlPin, steerControlPin;
    uint8_t cruiseControlPin, controlLockPin;

    uint16_t speedMax, speedCruise;
    uint16_t speedControl, steerControl;
    bool cruiseControl, controlLock;

public:
    A2D(uint8_t speedMaxPin, uint8_t speedCruisePin,
        uint8_t speedControlPin, uint8_t steerControlPin,
        uint8_t cruiseControlPin, uint8_t controlLockPin);
    bool begin();
    void collaborate();
    void process();
    bool getData(uint16_t &speedMax, uint16_t &speedCruise,
                 uint16_t &speedControl, uint16_t &steerControl,
                 bool &cruiseControl, bool &controlLock);
};
