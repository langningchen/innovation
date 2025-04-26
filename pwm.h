#pragma once

class PWM
{
protected:
    inline uint32_t ratio2Duty(double ratio);

    uint8_t pin;        // GPIO pin
    uint32_t freq;      // Frequency of PWM signal in hertz (Hz)
    uint8_t resolution; // Resolution for LEDC pin
    uint8_t channel;    // LEDC channel to attach to

public:
    PWM(uint8_t pin, uint32_t freq, uint8_t resolution, uint8_t channel);
    bool begin();
};
