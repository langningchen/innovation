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

#include <servo.hpp>

/**
 * @brief Servo constructor
 * @param aglRng Angle range in degrees (°), range is [1, 180]
 * @param minDuty Minimum duty cycle in percentage (%), range is [0, 100]
 * @param maxDuty Maximum duty cycle in percentage (%), range is [0, 100]
 * @note The angle range is from -aglRng to aglRng, and minDuty must be less than maxDuty
 * @see PWM::PWM()
 */
SERVO::SERVO(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel,
             uint8_t aglRng, uint8_t minDuty, uint8_t maxDuty)
    : PWM(pin, freq, resolution, channel)
{
    this->aglRng = constrain(aglRng, 1, 180);
    this->minDuty = constrain(minDuty, 0, 100);
    this->maxDuty = constrain(maxDuty, 0, 100);
}

/**
 * @brief Set the angle of the servo
 * @param angle Angle in degrees (°), range is [-aglRng, aglRng]
 * @return true if successful, false otherwise
 */
bool SERVO::setAngle(int16_t angle)
{
    angle = constrain(angle, -aglRng, aglRng);
    if (lastAngle != angle)
    {
        Serial.print("angle changed to ");
        Serial.print(angle);
        lastAngle = angle;
        return ledcWriteChannel(channel, map(angle, -aglRng, aglRng,
                                             ratio2Duty(minDuty), ratio2Duty(maxDuty)));
    }
    return false;
}
