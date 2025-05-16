#include <storage.hpp>

bool STORAGE::begin()
{
    if (!EEPROM.begin(sizeof(DATA)))
        return false;
    EEPROM.get(0, data);
    return true;
}
bool STORAGE::save()
{
    EEPROM.put(0, data);
    return EEPROM.commit();
}
uint8_t STORAGE::getControlTimeout() { return data.controlTimeout; }
bool STORAGE::setControlTimeout(uint8_t controlTimeout)
{
    data.controlTimeout = controlTimeout;
    return save();
}
uint8_t STORAGE::getMaxSpeed() { return data.maxSpeed; }
bool STORAGE::setMaxSpeed(uint8_t maxSpeed)
{
    data.maxSpeed = maxSpeed;
    return save();
}
