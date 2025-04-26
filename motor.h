#pragma once

#include <pwm.h>

class MOTOR : public PWM
{
public:
    MOTOR(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel);
    bool setSpeed(uint8_t speed);
};
