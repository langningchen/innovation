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
BATTERY::BATTERY(uint8_t address, float_t minVoltage, float_t maxVoltage)
    : address(address), minVoltage(minVoltage), maxVoltage(maxVoltage) {}

/**
 * @brief Initialize the battery monitor
 * @return true if successful, false otherwise
 * @note This function must be called before using any other functions in this class
 */
bool BATTERY::begin()
{
    ina226 = new INA226(address);
    return ina226->begin();
}

/**
 * @brief Get the battery voltage
 * @return Battery voltage in volts
 */
float_t BATTERY::getVoltage() { return ina226->getBusVoltage(); }

/**
 * @brief Get the battery percentage
 * @return Battery percentage in range [0, 100]
 */
uint8_t BATTERY::getPercentage()
{
    float_t percentage = (getVoltage() - minVoltage) / (maxVoltage - minVoltage) * 100;
    return constrain(percentage, 0, 100);
}
