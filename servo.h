#pragma once

#include <pwm.h>

class SERVO : public PWM
{
    uint8_t aglRng;  // Angle range in degrees (°)
    float_t minDuty; // Minimum duty cycle in percentage (%)
    float_t maxDuty; // Maximum duty cycle in percentage (%)
public:
    SERVO(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel,
          uint8_t aglRng, float_t minDuty, float_t maxDuty);
    bool setAngle(uint8_t angle);
};
