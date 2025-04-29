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
#ifdef CONTROL

#include <SPI.h>
#include <RF24.h>

#include <define.h>

RF24 radio(PIN_CE, PIN_CSN);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CSN);

    Serial.println("初始化 NRF24L01...");
    if (!radio.begin())
    {
        Serial.println("NRF24L01 初始化失败！");
        while (1)
            ;
    }

    radio.setChannel(76);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.enableAckPayload();
    radio.openWritingPipe(address);

    Serial.println("发送端初始化完成");
}

int cnt = 0;
int failCnt = 0;

void loop()
{
    if (Serial.available())
    {
        String text = Serial.readStringUntil('\n');
        text.trim();
        if (text.length() == 0)
            return;
        if (text.length() > 32)
        {
            Serial.println("输入的字符串过长，最大长度为32个字符！");
            return;
        }
        cnt++;
        Serial.print("发送数据: ");
        Serial.print(text);

        bool success = radio.write(text.c_str(), text.length() * sizeof(char));
        failCnt += !success;
        if (radio.isAckPayloadAvailable())
        {
            uint16_t ack = {0};
            radio.read(&ack, sizeof(ack));
            Serial.print("  收到应答: ");
            Serial.print(ack);
        }

        if (success)
            Serial.print("     ");
        else
            Serial.print(" 失败");

        Serial.println("  失败" + String(failCnt) + "/" + String(cnt) + String(failCnt * 1.0 / cnt * 100) + "%");
    }
}

#endif
