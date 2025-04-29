// Copyright (C) 2025 langningchen
//
// This file is part of innovation.
//
// innovation is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// innovation is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with innovation.  If not, see <https://www.gnu.org/licenses/>.

#include <switch.h>
#ifdef BOAT

#include <SPI.h>
#include <RF24.h>

#include <define.h>
#include <servo.h>
#include <motor.h>
#include <battery.h>

RF24 radio(PIN_CE, PIN_CSN);

// CAUTION! It seems that setting
// any channel to 0 will cause
// the configuration of first PWM channel
// overwritten by the second one
SERVO servo(PIN_PWM0, 100, 12, 1, 180, 5, 25);
MOTOR motor(PIN_PWM1, 5000, 8, 2);
BATTERY battery(PIN_ADC,
                14.0 / (270 + 40) * 40,  // 1.8064516129
                14.8 / (270 + 40) * 40); // 1.9096774194

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
    radio.setRetries(1, 5); // Delay 500us, 5 retries
    radio.enableAckPayload();
    radio.openReadingPipe(1, address);
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

    Serial.print("初始化电机...   ");
    if (!motor.begin())
    {
        Serial.println("失败！");
        while (1)
            ;
    }
    Serial.println("成功！");

    Serial.print("初始化电池...   ");
    if (!battery.begin())
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
    if (radio.available())
    {
        char type;
        radio.read(&type, sizeof(type));
        if (type == 's')
        {
            uint8_t angle;
            radio.read(&angle, sizeof(angle));
            Serial.print("舵机角度：");
            Serial.println(angle);
            servo.setAngle(angle);
        }
        else if (type == 'm')
        {
            uint8_t speed;
            radio.read(&speed, sizeof(speed));
            Serial.print("电机速度：");
            Serial.println(speed);
            motor.setSpeed(speed);
        }
        // 返回电池电压和电量
        uint16_t voltage = battery.getVoltage() * 1000;
        radio.writeAckPayload(1, &voltage, sizeof(voltage));
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
