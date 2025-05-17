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

#include <pwm.hpp>

/**
 * @brief Convert ratio to duty cycle value
 * @param ratio Input ratio, range is [0, 100]
 * @return Duty cycle value
 * @note The ratio is the percentage of the duty cycle
 */
inline uint32_t PWM::ratio2Duty(uint8_t ratio)
{
    ratio = constrain(ratio, 0, 100);
    return ratio * pow(2, resolution) / 100;
}

/**
 * @brief PWM constructor
 * @param pin GPIO pin
 * @param freq Frequency of PWM signal in hertz (Hz)
 * @param resolution Resolution for LEDC pin
 */
PWM::PWM(uint8_t pin, uint32_t freq, uint8_t resolution)
    : pin(pin), freq(freq), resolution(resolution) {}

/**
 * @brief Initialize the PWM
 * @return true if successful, false otherwise
 * @note This function must be called before using any other functions in this class
 */
bool PWM::begin()
{
    return ledcAttach(pin, freq, resolution);
}
