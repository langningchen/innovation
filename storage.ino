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

uint8_t STORAGE::getBackwardLimit() { return data.backwardLimit; }
bool STORAGE::setBackwardLimit(uint8_t backwardLimit)
{
    data.backwardLimit = backwardLimit;
    return save();
}

uint8_t STORAGE::getDirThreshold() { return data.dirThreshold; }
bool STORAGE::setDirThreshold(uint8_t dirThreshold)
{
    data.dirThreshold = dirThreshold;
    return save();
}

uint8_t STORAGE::getServoLimit() { return data.servoLimit; }
bool STORAGE::setServoLimit(uint8_t servoLimit)
{
    data.servoLimit = servoLimit;
    return save();
}

uint16_t STORAGE::getNetworkThreshold() { return data.networkThreshold; }
bool STORAGE::setNetworkThreshold(uint16_t networkThreshold)
{
    data.networkThreshold = networkThreshold;
    return save();
}

uint8_t STORAGE::getBatteryThreshold() { return data.batteryThreshold; }
bool STORAGE::setBatteryThreshold(uint8_t batteryThreshold)
{
    data.batteryThreshold = batteryThreshold;
    return save();
}

float_t STORAGE::getMpuXThreshold() { return data.mpuXThreshold; }
bool STORAGE::setMpuXThreshold(float_t mpuXThreshold)
{
    data.mpuXThreshold = mpuXThreshold;
    return save();
}

float_t STORAGE::getMpuYThreshold() { return data.mpuYThreshold; }
bool STORAGE::setMpuYThreshold(float_t mpuYThreshold)
{
    data.mpuYThreshold = mpuYThreshold;
    return save();
}

float_t STORAGE::getMpuZThreshold() { return data.mpuZThreshold; }
bool STORAGE::setMpuZThreshold(float_t mpuZThreshold)
{
    data.mpuZThreshold = mpuZThreshold;
    return save();
}

bool STORAGE::getLeftMotorDir() { return data.leftMotorDir; }
bool STORAGE::setLeftMotorDir(bool leftMotorDir)
{
    data.leftMotorDir = leftMotorDir;
    return save();
}

bool STORAGE::getRightMotorDir() { return data.rightMotorDir; }
bool STORAGE::setRightMotorDir(bool rightMotorDir)
{
    data.rightMotorDir = rightMotorDir;
    return save();
}

bool STORAGE::getEnableDS() { return data.enableDS; }
bool STORAGE::setEnableDS(bool enableDS)
{
    data.enableDS = enableDS;
    return save();
}

uint8_t STORAGE::getDSRate() { return data.dsRate; }
bool STORAGE::setDSRate(uint8_t dsRate)
{
    data.dsRate = dsRate;
    return save();
}

uint16_t STORAGE::getSpeedControlBasis() { return data.speedControlBasis; }
bool STORAGE::setSpeedControlBasis(uint16_t speedControlBasis)
{
    data.speedControlBasis = speedControlBasis;
    return save();
}

uint16_t STORAGE::getSteerControlBasis() { return data.steerControlBasis; }
bool STORAGE::setSteerControlBasis(uint16_t steerControlBasis)
{
    data.steerControlBasis = steerControlBasis;
    return save();
}
