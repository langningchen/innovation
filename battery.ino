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

#include <battery.hpp>

/**
 * @brief Battery constructor
 * @param pin Pin number for the battery monitor
 * @param minVoltage Minimum voltage of the battery
 * @param maxVoltage Maximum voltage of the battery
 */
BATTERY::BATTERY(uint8_t pin, float_t minVoltage, float_t maxVoltage)
    : pin(pin), minVoltage(minVoltage), maxVoltage(maxVoltage) {}

/**
 * * @brief Initialize the battery monitor
 * * @return true if successful, false otherwise
 * * @note This function must be called before using any other functions in this class
 */
bool BATTERY::begin()
{
    pinMode(pin, INPUT);
    analogReadResolution(ADC_RESOLUTION);
    return true;
}

/**
 * @brief Get the battery voltage
 * @return Battery voltage in volts
 * @note The voltage is calculated using a linear equation based on the ADC reading.
 *       The equation is derived from the file /docs/Battery.xlsx
 */
float_t BATTERY::getVoltage()
{
    float_t analogData = 0;
    for (int i = 0; i < 10; i++)
    {
        analogData += analogRead(pin);
        delay(0);
    }
    analogData /= 10;
    return (86.954 * analogData + 1539.4) / 100000;
}

/**
 * @brief Get the battery percentage
 * @return Battery percentage in range [0, 100]
 */
uint8_t BATTERY::getPercentage()
{
    float_t percentage = (getVoltage() - minVoltage) / (maxVoltage - minVoltage) * 100;
    return constrain(percentage, 0, 100);
}
