#pragma once

#include <pwm.h>

class SERVO : public PWM
{
    uint8_t aglRng;
    uint8_t minDuty;
    uint8_t maxDuty;

public:
    SERVO(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel,
          uint8_t aglRng, uint8_t minDuty, uint8_t maxDuty);
    bool setAngle(uint8_t angle);
};
