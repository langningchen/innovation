#include "servo.h"

inline int SERVO::timeToPWM(double time) { return time * this->freq * pow(2, this->res) / 1000; }
int SERVO::angleToPWM(int angle)
{
    if (angle < -this->aglRng || angle > this->aglRng)
        return -1;
    return map(angle, -this->aglRng, this->aglRng, timeToPWM(0.5), timeToPWM(2.5));
}

SERVO::SERVO(int pin, int freq, int res, int aglRng) : pin(pin), freq(freq), res(res), aglRng(aglRng) {}

bool SERVO::begin()
{
    if (!ledcAttach(this->pin, this->freq, this->res))
        return false;
    ledcWrite(this->pin, angleToPWM(0));
    return true;
}

void SERVO::setAngle(int angle)
{
    int pwmValue = angleToPWM(angle);
    if (pwmValue == -1)
    {
        Serial.println("无效的角度值");
        return;
    }
    ledcWrite(pin, pwmValue);
    Serial.print("舵机转向: ");
    Serial.print(angle);
    Serial.println(" 度");
}
