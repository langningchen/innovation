#include <pwm.h>

/// @brief Convert ratio to duty cycle value
/// @param ratio Input ratio, range is from 0 to 1
/// @return Duty cycle value
/// @note The ratio is the percentage of the duty cycle, where 0 is 0% and 1 is 100%
inline uint32_t PWM::ratio2Duty(double ratio)
{
    assert(ratio >= 0 && ratio <= 1);
    return ratio * pow(2, this->resolution);
}

/// @brief PWM constructor
/// @param pin Pin number
/// @param freq Frequency in hertz (Hz)
/// @param resolution Resolution
PWM::PWM(uint8_t pin, uint32_t freq, uint8_t resolution)
    : pin(pin), freq(freq), resolution(resolution) {}

/// @brief Initialize the PWM
/// @return true if successful, false otherwise
/// @note This function must be called before using any other functions in this class
bool PWM::begin()
{
    return ledcAttach(this->pin, this->freq, this->resolution);
}
