#include <storage.hpp>

/**
 * @brief Initialize the STORAGE
 * @return true if successful, false otherwise
 */
bool STORAGE::begin()
{
    if (!EEPROM.begin(sizeof(DATA)))
        return false;
    EEPROM.get(0, data);
    return true;
}
/**
 * @brief Save the storage
 * @return true if successful, false otherwise
 */
bool STORAGE::save()
{
    EEPROM.put(0, data);
    return EEPROM.commit();
}
/**
 * @brief Reset the storage to default values
 * @return true if successful, false otherwise
 */
bool STORAGE::reset()
{
    data = defaultData;
    return save();
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

int8_t STORAGE::getLeftServoDelta() { return data.leftServoDelta; }
bool STORAGE::setLeftServoDelta(int8_t leftServoDelta)
{
    data.leftServoDelta = leftServoDelta;
    return save();
}

int8_t STORAGE::getRightServoDelta() { return data.rightServoDelta; }
bool STORAGE::setRightServoDelta(int8_t rightServoDelta)
{
    data.rightServoDelta = rightServoDelta;
    return save();
}

int8_t STORAGE::getLeftMotorDelta() { return data.leftMotorDelta; }
bool STORAGE::setLeftMotorDelta(int8_t leftMotorDelta)
{
    data.leftMotorDelta = leftMotorDelta;
    return save();
}

int8_t STORAGE::getRightMotorDelta() { return data.rightMotorDelta; }
bool STORAGE::setRightMotorDelta(int8_t rightMotorDelta)
{
    data.rightMotorDelta = rightMotorDelta;
    return save();
}
