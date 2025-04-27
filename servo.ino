#include <servo.h>

/**
 * @brief Servo constructor
 * @param aglRng Angle range in degrees (°), range is [1, 360]
 * @param minDuty Minimum duty cycle in percentage (%), range is [0, 100]
 * @param maxDuty Maximum duty cycle in percentage (%), range is [0, 100]
 * @note The angle range is from 0 to aglRng, and minDuty must be less than maxDuty
 * @see PWM::PWM()
 */
SERVO::SERVO(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel,
             uint8_t aglRng, uint8_t minDuty, uint8_t maxDuty)
    : PWM(pin, freq, resolution, channel)
{
    assert(aglRng >= 1), assert(aglRng <= 360);
    this->aglRng = aglRng;
    assert(minDuty >= 0), assert(minDuty <= 100);
    assert(maxDuty >= 0), assert(maxDuty <= 100);
    assert(minDuty < maxDuty);
    this->minDuty = minDuty;
    this->maxDuty = maxDuty;
}

/**
 * @brief Set the angle of the servo
 * @param angle Angle in degrees (°), range is [0, aglRng]
 * @return true if successful, false otherwise
 */
bool SERVO::setAngle(uint8_t angle)
{
    assert(angle >= 0), assert(angle <= aglRng);
    return ledcWriteChannel(channel, map(angle, 0, aglRng,
                                         ratio2Duty(minDuty), ratio2Duty(maxDuty)));
}
