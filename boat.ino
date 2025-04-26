#include <switch.h>
#ifdef BOAT

#include <SPI.h>
#include <RF24.h>

#include <define.h>
#include <servo.h>
#include <motor.h>

RF24 radio(PIN_CE, PIN_CSN);

// CAUTION! It seems that setting
// any channel to 0 will cause
// the configuration of first PWM channel
// overwritten by the second one
SERVO servo(PIN_PWM0, 100, 12, 1, 180, 5, 25);
MOTOR motor(PIN_PWM1, 5000, 8, 2);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CSN);

    // Serial.print("初始化 NRF24L01...   ");
    // if (!radio.begin())
    // {
    //     Serial.println("失败！");
    //     while (1)
    //         ;
    // }
    // radio.setChannel(76);
    // radio.setDataRate(RF24_250KBPS);
    // radio.setPALevel(RF24_PA_LOW);
    // radio.setAutoAck(true);
    // radio.openReadingPipe(0, address);
    // radio.startListening();
    // Serial.println("成功！");

    Serial.print("初始化舵机...   ");
    if (!servo.begin())
    {
        Serial.println("失败！");
        while (1)
            ;
    }
    Serial.println("成功！");

    Serial.print("初始化电机...   ");
    if (!motor.begin())
    {
        Serial.println("失败！");
        while (1)
            ;
    }
    Serial.println("成功！");

    Serial.println("接收端初始化完成，等待数据...");
}

clock_t lst_msg = 0;
clock_t lst_chk = 0;

void loop()
{
    if (Serial.available())
    {
        String type = Serial.readStringUntil(' ');
        auto Data = Serial.readStringUntil('\n').toInt();
        Serial.print(type == "servo" ? "servo" : "motor");
        Serial.print(" set to ");
        Serial.print(Data);
        Serial.print("...   ");
        if ((type == "servo" ? servo.setAngle(Data) : motor.setSpeed(Data)))
            Serial.println("success");
        else
            Serial.println("failed");
        lst_msg = clock();
    }
    else
    {
        if (clock() - lst_msg > CONTROL_TIMEOUT && clock() - lst_chk > 1000)
        {
            Serial.println(lst_msg == 0 ? "等待遥控器信号中..." : "丢失遥控器信号，等待中...");
            lst_chk = clock();
        }
    }
}

#endif
