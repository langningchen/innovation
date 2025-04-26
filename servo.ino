#include "servo.h"

/// @brief Servo constructor
/// @param aglRng Angle range in degrees (°)
/// @param minDuty Minimum duty cycle in percentage (%)
/// @param maxDuty Maximum duty cycle in percentage (%)
/// @note The angle range is from 0 to aglRng
/// @see PWM::PWM()
SERVO::SERVO(uint8_t pin, uint32_t freq, uint8_t resolution,
             uint8_t aglRng, uint8_t minDuty, uint8_t maxDuty)
    : PWM(pin, freq, resolution)
{
    assert(aglRng > 0 && aglRng <= 360);
    this->aglRng = aglRng;
    assert(minDuty >= 0 && minDuty <= 100);
    assert(maxDuty >= 0 && maxDuty <= 100);
    assert(minDuty < maxDuty);
    this->minDuty = minDuty;
    this->maxDuty = maxDuty;
}

/// @brief Set the angle of the SERVO
/// @param angle Angle in degrees (°)
/// @return true if successful, false otherwise
bool SERVO::setAngle(uint8_t angle)
{
    assert(angle >= 0 && angle <= this->aglRng);
    return ledcWrite(this->pin, map(angle,
                                    0, this->aglRng,
                                    ratio2Duty(this->minDuty), ratio2Duty(this->maxDuty)));
}
