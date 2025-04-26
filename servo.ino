#include "servo.h"

/// @brief Servo constructor
/// @param aglRng Angle range in degrees (°)
/// @note The angle range is from 0 to aglRng
/// @see PWM::PWM()
SERVO::SERVO(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t aglRng)
    : PWM(pin, freq, resolution)
{
    assert(aglRng > 0 && aglRng <= 360);
    this->aglRng = aglRng;
}

/// @brief Set the angle of the SERVO
/// @param angle Angle in degrees (°)
/// @return true if successful, false otherwise
bool SERVO::setAngle(uint8_t angle)
{
    assert(angle >= 0 && angle <= this->aglRng);
    return ledcWrite(this->pin, map(angle,
                                    0, this->aglRng,
                                    ratio2Duty(0.05), ratio2Duty(0.25)));
}
