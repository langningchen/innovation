#pragma once

class PWM
{
protected:
    inline uint32_t ratio2Duty(double ratio);

    uint8_t pin;        // Pin number
    uint32_t freq;      // Frequency in hertz (Hz)
    uint8_t resolution; // Resolution

public:
    PWM(uint8_t pin, uint32_t freq, uint8_t resolution);
    bool begin();
    bool setAngle(uint8_t angle);
};
