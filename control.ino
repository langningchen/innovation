#include "switch.h"
#ifdef CONTROL

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "define.h"

#include <string>

RF24 radio(CE, CSN);

void setup()
{
    Serial.begin(115200);
    // delay(1000);

    // 初始化 SPI
    SPI.begin(SCK, MISO, MOSI, CSN);

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
    radio.setAutoAck(true);
    radio.openWritingPipe(address);
    radio.stopListening();

    Serial.println("发送端初始化完成");
}

int cnt = 0;
int failCnt = 0;

void loop()
{
    std::string text = "Hello";
    text += std::to_string(cnt);
    cnt++;
    Serial.print("发送中: ");
    Serial.print(text.c_str());

    bool success = radio.write(text.c_str(), text.size());
    failCnt += !success;
    if (success)
        Serial.print("     ");
    else
        Serial.print(" 失败");

    Serial.println(("  失败率" + std::to_string(failCnt * 1.0 / cnt * 100) + "%").c_str());

    delay(100);
}

#endif
