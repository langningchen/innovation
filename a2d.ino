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
 * @param enableCruisePin cruise control pin
 * @param enableLockPin control lock pin
 */
A2D::A2D(uint8_t speedMaxPin, uint8_t speedCruisePin,
         uint8_t speedControlPin, uint8_t steerControlPin,
         uint8_t enableCruisePin, uint8_t enableLockPin)
    : speedMaxPin(speedMaxPin), speedCruisePin(speedCruisePin),
      speedControlPin(speedControlPin), steerControlPin(steerControlPin),
      enableCruisePin(enableCruisePin), enableLockPin(enableLockPin) {}

/**
 * @brief Initialize the A2D
 */
void A2D::begin()
{
    pinMode(speedMaxPin, INPUT), pinMode(speedCruisePin, INPUT);
    pinMode(speedControlPin, INPUT), pinMode(steerControlPin, INPUT);
    pinMode(enableCruisePin, INPUT), pinMode(enableLockPin, INPUT);

    analogReadResolution(ADC_RESOLUTION);

    speedMaxMin = speedCruiseMin = speedControlMin = steerControlMin = 0;
    speedMaxMax = speedCruiseMax = speedControlMax = steerControlMax = (1 << ADC_RESOLUTION) - 1;
    speedControlBasis = steerControlBasis = 1 << (ADC_RESOLUTION - 1);

    speedMax = analogRead(speedMaxPin), speedCruise = analogRead(speedCruisePin);
    speedControl = analogRead(speedControlPin), steerControl = analogRead(steerControlPin);
    enableCruise = digitalRead(enableCruisePin), enableLock = digitalRead(enableLockPin);
}

/**
 * @brief Reset the A2D min and max values
 */
void A2D::reset()
{
    speedMaxMin = speedCruiseMin = speedControlMin = steerControlMin = 0;
    speedMaxMax = speedCruiseMax = speedControlMax = steerControlMax = (1 << ADC_RESOLUTION) - 1;
}

/**
 * @brief Set current values as the basis
 */
void A2D::setBasis() { speedControlBasis = speedControl, steerControlBasis = steerControl; }

/**
 * @brief Record a new data
 * @param collaborate whether to collaborate the data
 * @details This function will record the new data,
 * if the collaborate parameter is true, it will update the min and max values
 */
void A2D::process(bool collaborate)
{
    uint16_t newSpeedMaxData = analogRead(speedMaxPin);
    uint16_t newSpeedCruiseData = analogRead(speedCruisePin);
    uint16_t newSpeedControlData = analogRead(speedControlPin);
    uint16_t newSteerControlData = analogRead(steerControlPin);
    speedMax = speedMax * (1 - speedMaxFilterCoef) + newSpeedMaxData * speedMaxFilterCoef;
    speedCruise = speedCruise * (1 - speedCruiseFilterCoef) + newSpeedCruiseData * speedCruiseFilterCoef;
    speedControl = speedControl * (1 - speedControlFilterCoef) + newSpeedControlData * speedControlFilterCoef;
    steerControl = steerControl * (1 - steerControlFilterCoef) + newSteerControlData * steerControlFilterCoef;
    enableCruise = !digitalRead(enableCruisePin), enableLock = digitalRead(enableLockPin);

    if (collaborate)
    {
        speedMaxMax = max(speedMaxMax, speedMax), speedMaxMin = min(speedMaxMin, speedMax);
        speedCruiseMax = max(speedCruiseMax, speedCruise), speedCruiseMin = min(speedCruiseMin, speedCruise);
        speedControlMax = max(speedControlMax, speedControl), speedControlMin = min(speedControlMin, speedControl);
        steerControlMax = max(steerControlMax, steerControl), steerControlMin = min(steerControlMin, steerControl);
    }
}

/**
 * @brief Get all the data
 * @param speedMax output speed max
 * @param speedCruise output speed cruise
 * @param speedControl output speed control
 * @param steerControl output steer control
 * @param enableCruise output whether cruise control is enabled
 * @param enableLock output whether control lock is enabled
 * @details This function will get all the data and convert it to the range of [0, 100]
 */
void A2D::getData(uint8_t &speedMax, uint8_t &speedCruise,
                  int8_t &speedControl, int8_t &steerControl,
                  bool &enableCruise, bool &enableLock)
{
    uint16_t constrainedSpeedMax = constrain(this->speedMax, speedMaxMin, speedMaxMax);
    uint16_t constrainedSpeedCruise = constrain(this->speedCruise, speedCruiseMin, speedCruiseMax);
    uint16_t constrainedSpeedControl = constrain(this->speedControl, speedControlMin, speedControlMax);
    uint16_t constrainedSteerControl = constrain(this->steerControl, steerControlMin, steerControlMax);
    speedMax = 100 - map(constrainedSpeedMax, speedMaxMin, speedMaxMax, 0, 100);
    speedCruise = 100 - map(constrainedSpeedCruise, speedCruiseMin, speedCruiseMax, 0, 100);
    if (constrainedSpeedControl >= speedControlBasis)
        speedControl = map(constrainedSpeedControl, speedControlBasis, speedControlMax, 0, 100);
    else
        speedControl = map(constrainedSpeedControl, speedControlMin, speedControlBasis, -100, 0);
    if (constrainedSteerControl >= steerControlBasis)
        steerControl = map(constrainedSteerControl, steerControlBasis, steerControlMax, 0, 100);
    else
        steerControl = map(constrainedSteerControl, steerControlMin, steerControlBasis, -100, 0);
    enableCruise = this->enableCruise, enableLock = this->enableLock;
}
