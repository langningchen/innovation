#include <motor.h>

/**
 * @brief Motor constructor
 * @see PWM::PWM()
 */
MOTOR::MOTOR(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel)
    : PWM(pin, freq, resolution, channel) {}

/**
 * @brief Set the speed of the motor
 * @param speed Speed in percentage (%), range is [0, 100]
 * @return true if successful, false otherwise
 */
bool MOTOR::setSpeed(uint8_t speed)
{
    assert(speed >= 0), assert(speed <= 100);
    return ledcWriteChannel(channel, map(speed, 0, 100, 0, pow(2, resolution)));
}
