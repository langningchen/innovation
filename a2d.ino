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

#include <a2d.hpp>

/**
 * @brief A2D constructor
 * @param speedMaxPin speed max pin
 * @param speedCruisePin speed cruise pin
 * @param speedControlPin speed control pin
 * @param steerControlPin steer control pin
 * @param cruiseControlPin cruise control pin
 * @param controlLockPin control lock pin
 */
A2D::A2D(uint8_t speedMaxPin, uint8_t speedCruisePin,
         uint8_t speedControlPin, uint8_t steerControlPin,
         uint8_t cruiseControlPin, uint8_t controlLockPin)
    : speedMaxPin(speedMaxPin), speedCruisePin(speedCruisePin),
      speedControlPin(speedControlPin), steerControlPin(steerControlPin),
      cruiseControlPin(cruiseControlPin), controlLockPin(controlLockPin) {}

/**
 * @brief Initialize the A2D
 * @return true if successful, false otherwise
 */
bool A2D::begin()
{
    pinMode(speedMaxPin, INPUT), pinMode(speedCruisePin, INPUT);
    pinMode(speedControlPin, INPUT), pinMode(steerControlPin, INPUT);
    pinMode(cruiseControlPin, INPUT), pinMode(controlLockPin, INPUT);

    analogReadResolution(ADC_RESOLUTION);

    speedMax = analogRead(speedMaxPin), speedCruise = analogRead(speedCruisePin);
    speedControl = analogRead(speedControlPin), steerControl = analogRead(steerControlPin);
    cruiseControl = digitalRead(cruiseControlPin), controlLock = digitalRead(controlLockPin);
    return true;
}

/**
 * @brief Collaborate the A2D
 * @return true if successful, false otherwise
 */
void A2D::collaborate()
{
    for (uint16_t i = 0; i < (1 << 10); i++)
        process();
    speedControlBasis = speedControl, steerControlBasis = steerControl;
}

/**
 * @brief Record a new data
 */
void A2D::process()
{
    speedMax = speedMax * (1 - speedMaxFilterCoef) + analogRead(speedMaxPin) * speedMaxFilterCoef;
    speedCruise = speedCruise * (1 - speedCruiseFilterCoef) + analogRead(speedCruisePin) * speedCruiseFilterCoef;
    speedControl = speedControl * (1 - speedControlFilterCoef) + analogRead(speedControlPin) * speedControlFilterCoef;
    steerControl = steerControl * (1 - steerControlFilterCoef) + analogRead(steerControlPin) * steerControlFilterCoef;
    cruiseControl = digitalRead(cruiseControlPin), controlLock = digitalRead(controlLockPin);
}

/**
 * @brief Get all the data
 * @param speedMax output speed max
 * @param speedCruise output speed cruise
 * @param speedControl output speed control
 * @param steerControl output steer control
 * @param cruiseControl output cruise control
 * @param controlLock output control lock
 * @return
 */
bool A2D::getData(uint16_t &speedMax, uint16_t &speedCruise,
                  uint16_t &speedControl, uint16_t &steerControl,
                  bool &cruiseControl, bool &controlLock)
{
    speedMax = this->speedMax, speedCruise = this->speedCruise;
    speedControl = this->speedControl - speedControlBasis, steerControl = this->steerControl - steerControlBasis;
    cruiseControl = this->cruiseControl, controlLock = this->controlLock;
    return true;
}
