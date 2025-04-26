#pragma once

class PWM
{
protected:
    inline uint32_t ratio2Duty(uint8_t ratio);

    uint8_t pin;
    uint32_t freq;
    uint8_t resolution;
    uint8_t channel;

public:
    PWM(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel);
    bool begin();
};
