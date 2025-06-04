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
 * @brief Get the direction
 * @details This function will get the direction based on the speed control and steer control
 * @return The direction of the vehicle
 */
DIR A2D::getDirection()
{
    int8_t upPercent = (speedControl - speedControlBasis) * 100 / (speedControlMax - speedControlBasis);
    int8_t downPercent = (speedControlBasis - speedControl) * 100 / (speedControlBasis - speedControlMin);
    int8_t rightPercent = (steerControl - steerControlBasis) * 100 / (steerControlMax - steerControlBasis);
    int8_t leftPercent = (steerControlBasis - steerControl) * 100 / (steerControlBasis - steerControlMin);
    int8_t maxPercent = max(max(upPercent, downPercent), max(leftPercent, rightPercent));
    if (maxPercent < storage.getDirThreshold())
        return NONE;
    if (upPercent == maxPercent)
        return UP;
    if (downPercent == maxPercent)
        return DOWN;
    if (rightPercent == maxPercent)
        return RIGHT;
    if (leftPercent == maxPercent)
        return LEFT;
    return NONE;
}

/**
 * @brief Update the direction based on the speed and steer control
 */
void A2D::updateDirection()
{
    auto currentDirection = getDirection();
    if (currentDirection != lastDirection)
    {
        if (onDirectionEnd)
            onDirectionEnd(lastDirection);
        lastDirection = currentDirection;
    }
}

/**
 * @brief A2D constructor
 * @param configPin speed max pin
 * @param speedCruisePin speed cruise pin
 * @param speedControlPin speed control pin
 * @param steerControlPin steer control pin
 * @param enableCruisePin cruise control pin
 * @param enableLockPin control lock pin
 */
A2D::A2D(uint8_t configPin, uint8_t speedCruisePin,
         uint8_t speedControlPin, uint8_t steerControlPin,
         uint8_t enableCruisePin, uint8_t enableLockPin,
         STORAGE &storage)
    : configPin(configPin), speedCruisePin(speedCruisePin),
      speedControlPin(speedControlPin), steerControlPin(steerControlPin),
      enableCruisePin(enableCruisePin), enableLockPin(enableLockPin),
      storage(storage) {}

/**
 * @brief Initialize the A2D
 */
void A2D::begin()
{
    pinMode(configPin, INPUT), pinMode(speedCruisePin, INPUT);
    pinMode(speedControlPin, INPUT), pinMode(steerControlPin, INPUT);
    pinMode(enableCruisePin, INPUT), pinMode(enableLockPin, INPUT);

    analogReadResolution(ADC_RESOLUTION);

    configMin = speedCruiseMin = speedControlMin = steerControlMin = 0;
    configMax = speedCruiseMax = speedControlMax = steerControlMax = (1 << ADC_RESOLUTION) - 1;
    
    // 从存储中读取校准值
    speedControlBasis = storage.getSpeedControlBasis();
    steerControlBasis = storage.getSteerControlBasis();
    
    // 如果读取的值超出范围，则使用默认值
    if (speedControlBasis == 0 || speedControlBasis >= (1 << ADC_RESOLUTION))
        speedControlBasis = 1 << (ADC_RESOLUTION - 1);
    if (steerControlBasis == 0 || steerControlBasis >= (1 << ADC_RESOLUTION))
        steerControlBasis = 1 << (ADC_RESOLUTION - 1);

    config = analogRead(configPin), speedCruise = analogRead(speedCruisePin);
    speedControl = analogRead(speedControlPin), steerControl = analogRead(steerControlPin);
    enableCruise = digitalRead(enableCruisePin), enableLock = digitalRead(enableLockPin);
}

/**
 * @brief Reset the A2D min and max values
 */
void A2D::reset()
{
    configMin = speedCruiseMin = speedControlMin = steerControlMin = 0;
    configMax = speedCruiseMax = speedControlMax = steerControlMax = (1 << ADC_RESOLUTION) - 1;
}

/**
 * @brief Record a new data
 * @param collaborate whether to collaborate the data
 * @details This function will record the new data,
 * if the collaborate parameter is true, it will update the min and max values
 */
void A2D::process(bool noUpdateDirection)
{
    uint16_t newConfigData = analogRead(configPin);
    uint16_t newSpeedCruiseData = analogRead(speedCruisePin);
    uint16_t newSpeedControlData = analogRead(speedControlPin);
    uint16_t newSteerControlData = analogRead(steerControlPin);
    config = config * (1 - configFilterCoef) + newConfigData * configFilterCoef;
    speedCruise = speedCruise * (1 - speedCruiseFilterCoef) + newSpeedCruiseData * speedCruiseFilterCoef;
    speedControl = speedControl * (1 - speedControlFilterCoef) + newSpeedControlData * speedControlFilterCoef;
    steerControl = steerControl * (1 - steerControlFilterCoef) + newSteerControlData * steerControlFilterCoef;
    enableCruise = !digitalRead(enableCruisePin), enableLock = digitalRead(enableLockPin);
    if (!noUpdateDirection)
        updateDirection();
}

/**
 * @brief Get all the data
 * @param config output speed max
 * @param speedCruise output speed cruise
 * @param speedControl output speed control
 * @param steerControl output steer control
 * @param enableCruise output whether cruise control is enabled
 * @param enableLock output whether control lock is enabled
 * @details This function will get all the data and convert it to the range of [-100, 100] or [0, 1]
 */
void A2D::getData(uint8_t &config, uint8_t &speedCruise,
                  int8_t &speedControl, int8_t &steerControl,
                  bool &enableCruise, bool &enableLock)
{
    uint16_t constrainedConfig = constrain(this->config, configMin, configMax);
    uint16_t constrainedSpeedCruise = constrain(this->speedCruise, speedCruiseMin, speedCruiseMax);
    uint16_t constrainedSpeedControl = constrain(this->speedControl, speedControlMin, speedControlMax);
    uint16_t constrainedSteerControl = constrain(this->steerControl, steerControlMin, steerControlMax);
    config = 100 - map(constrainedConfig, configMin, configMax, 0, 100);
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

/**
 * @brief Update the min and max values
 */
void A2D::updateMinMax()
{
    configMax = max(configMax, config), configMin = min(configMin, config);
    speedCruiseMax = max(speedCruiseMax, speedCruise), speedCruiseMin = min(speedCruiseMin, speedCruise);
    speedControlMax = max(speedControlMax, speedControl), speedControlMin = min(speedControlMin, speedControl);
    steerControlMax = max(steerControlMax, steerControl), steerControlMin = min(steerControlMin, steerControl);
}

/**
 * @brief Set the callback function for direction end
 * @param callback callback function, it will be called when the direction changes
 * @details The callback function will be called with the last direction as the parameter
 */
void A2D::setOnDirectionEnd(std::function<void(DIR)> callback) { onDirectionEnd = callback; }

/**
 * @brief Calibrate the A2D
 * @param a2d the A2D object to calibrate
 */
void A2D::calibrate(Adafruit_SSD1306 &display)
{
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.println("Calibrating...");
    display.println("Please do not touch the joystick");
    display.display();
    for (int i = 0; i < display.width(); i++)
    {
        process(true);
        display.drawFastHLine(i, 32, 3, WHITE);
        display.display();
        delay(10);
    }
    speedControlBasis = speedControl;
    steerControlBasis = steerControl;
    
    // Save the calibrated values to storage
    storage.setSpeedControlBasis(speedControlBasis);
    storage.setSteerControlBasis(steerControlBasis);
    
    display.display();
}
