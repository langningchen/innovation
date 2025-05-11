#include <oled.hpp>

OLED::OLED(uint8_t address, uint8_t width, uint8_t height)
    : address(address), width(width), height(height),
      display(width, height, &Wire, -1) {}

bool OLED::begin()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, address))
    return false;
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE);
  display.println("Innovation");
  display.setTextColor(WHITE, BLACK);
  display.println();
  display.println("Compile time");
  display.println(__DATE__);
  display.println(__TIME__);
  display.display();
  return true;
}
