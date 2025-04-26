#pragma once

#include <pwm.h>

class SERVO : public PWM
{
    uint8_t aglRng;  // Angle range in degrees (°)
    uint8_t minDuty; // Minimum duty cycle in percentage (%)
    uint8_t maxDuty; // Maximum duty cycle in percentage (%)
public:
    SERVO(uint8_t pin, uint32_t freq, uint8_t resolution,
          uint8_t aglRng, uint8_t minDuty, uint8_t maxDuty);
    bool setAngle(uint8_t angle);
};
