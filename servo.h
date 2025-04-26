#pragma once

class SERVO
{
    inline int timeToPWM(double time);
    int angleToPWM(int angle);

    int pin, freq, res, aglRng;

public:
    SERVO(int pin, int freq, int res, int aglRng);
    bool begin();
    void setAngle(int angle);
};
