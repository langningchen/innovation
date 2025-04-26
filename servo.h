#pragma once

#include <pwm.h>

class SERVO : public PWM
{
    uint8_t aglRng; // Angle range in degrees (°)
public:
    SERVO(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t aglRng);
    bool setAngle(uint8_t angle);
};
