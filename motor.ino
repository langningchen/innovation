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

#include <motor.hpp>

/**
 * @brief Motor constructor
 * @param dirPin Direction pin
 * @param dir Direction, boolean
 * @see PWM::PWM()
 */
MOTOR::MOTOR(uint8_t pin, uint8_t dirPin, uint32_t freq, uint8_t resolution, uint8_t channel, bool dir)
    : PWM(pin, freq, resolution, channel), dirPin(dirPin), dir(dir) {}

/**
 * @brief Set the speed of the motor
 * @param speed Speed in percentage (%), range is [0, 100]
 * @return true if successful, false otherwise
 */
bool MOTOR::setSpeed(int8_t speed)
{
    // assert(speed >= 0), assert(speed <= 100);
    speed = constrain(speed, -100, 100);
    digitalWrite(dirPin, (speed < 0) ^ dir);
    return ledcWriteChannel(channel, map(abs(speed), 0, 100, 0, pow(2, resolution)));
}
