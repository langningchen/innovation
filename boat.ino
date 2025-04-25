#include "switch.h"
#ifdef BOAT

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "define.h"

RF24 radio(CE, SS);
const byte address[6] = "765FA";

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // 初始化 SPI
    SPI.begin(SCK, MISO, MOSI, SS);

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
    radio.openReadingPipe(0, address);
    radio.startListening();

    Serial.println("接收端初始化完成，等待数据...");
}

void loop()
{
    if (radio.available())
    {
        char receivedText[32] = {0};
        radio.read(&receivedText, sizeof(receivedText));
        Serial.print("接收到数据: ");
        Serial.println(receivedText);
    }
}

#endif
