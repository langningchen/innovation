#include <storage.hpp>

void STORAGE::load() { EEPROM.get(0, data); }
void STORAGE::save()
{
    EEPROM.put(0, data);
    EEPROM.commit();
}
uint8_t STORAGE::getControlTimeout() { return data.controlTimeout; }
void STORAGE::setControlTimeout(uint8_t controlTimeout) { data.controlTimeout = controlTimeout; }
