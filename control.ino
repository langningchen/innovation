#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 9

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 初始化 SPI
  SPI.begin(10, 12, 11, 9); // SCK, MISO, MOSI, SS

  Serial.println("初始化 NRF24L01...");
  if (!radio.begin()) {
    Serial.println("NRF24L01 初始化失败！");
    while (1);
  }

  radio.setChannel(76);
  radio.setDataRate(RF24_250KBPS); // 更强抗干扰
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(true);
  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("发送端初始化完成");
}

void loop() {
  const char text[] = "HellAAAA NRF24L01!";
  Serial.print("发送中: ");
  Serial.println(text);

  bool success = radio.write(&text, sizeof(text));

  if (success) {
    Serial.println("发送成功");
  } else {
    Serial.println("发送失败");
  }

  delay(1000);
}