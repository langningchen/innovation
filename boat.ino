#include "switch.h"
#ifdef BOAT

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "define.h"
#include "servo.h"

RF24 radio(PIN_CE, PIN_CSN);

SERVO servo(PIN_PWM0, 100, 12, 90);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CSN);

    Serial.print("初始化 NRF24L01...   ");
    if (!radio.begin())
    {
        Serial.println("失败！");
        while (1)
            ;
    }
    radio.setChannel(76);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.setAutoAck(true);
    radio.openReadingPipe(0, address);
    radio.startListening();
    Serial.println("成功！");

    Serial.print("初始化舵机...   ");
    if (!servo.begin())
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
        String receivedText = Serial.readStringUntil('\n');
        Serial.print("接收到数据: ");
        Serial.println(receivedText);
        servo.setAngle(receivedText.toInt());

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
