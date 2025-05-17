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

#include <switch.hpp>
#ifdef CONTROL

#include <define.hpp>
#include <network.hpp>
#include <messages.hpp>
#include <a2d.hpp>
#include <oled.hpp>
#include <storage.hpp>

NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_IRQ, PIN_RESET, PIN_BUSY);
A2D a2d(PIN_SPEED_MAX, PIN_SPEED_CRUISE, PIN_SPEED_CONTROL, PIN_STEER_CONTROL, PIN_CRUISE_CONTROL, PIN_CONTROL_LOCK);
STORAGE storage;
OLED oled(OLED_ADDRESS, OLED_WIDTH, OLED_HEIGHT, storage);

uint32_t lastMsg;
uint8_t config, speedCruise;
int8_t motorControl, servoControl;
bool enableCruise, enableLock;

/**
 * @brief Update the data from the original data
 * @param speedCruise in
 * @param speedControl in/out
 * @param steerControl in/out
 * @param enableCruise in
 * @param enableLock in
 * @details according to the the lock and cruise pin status to update the control speed and steer for network.
 * limit the speed to MaxSpeed or the cruise speed, etc.
 */
void updateData(uint8_t &speedCruise,
                int8_t &speedControl, int8_t &steerControl,
                bool &enableCruise, bool &enableLock);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

    if (!Wire.begin(PIN_OLED_SDA, PIN_OLED_SCL))
    {
        Serial.println("I2C initialization failed");
        while (1)
            ;
    }

    if (!network.begin())
    {
        Serial.println("Network initialization failed");
        while (1)
            ;
    }
    network.setClient();

    if (!oled.begin())
    {
        Serial.println("OLED initialization failed");
        while (1)
            ;
    }

    if (!storage.begin())
    {
        Serial.println("Storage initialization failed");
        while (1)
            ;
    }

    a2d.begin();
    a2d.setOnDirectionEnd([](DIR dir)
                          { oled.dirInput(dir); });
    Serial.println("Control initialization completed");
    lastMsg = millis();
}

void loop()
{
    a2d.process();
    oled.process();

    if (millis() - lastMsg > MSG_INTERVAL)
    {
        lastMsg += MSG_INTERVAL;

        static uint8_t lastConfig = 0;
        a2d.getData(config, speedCruise, motorControl, servoControl, enableCruise, enableLock);
        oled.switchPage(enableLock ? OLED::PAGE::PAGE_CONFIG : OLED::PAGE::PAGE_STATUS);
        if (lastConfig != config)
        {
            oled.knobInput(config);
            lastConfig = config;
        }

        // updateData(speedCruise, motorControl, servoControl, enableCruise, enableLock);
        if (enableCruise)
            motorControl = speedCruise;
        int8_t motorSpeed = motorControl * (motorControl < 0 ? BACKWARD_LIMIT : storage.getMaxSpeed() / 100.0);
        servoControl *= 60.0 / SERVO_RANGE;
        if (motorSpeed > 0)
            servoControl *= 1.0 - (motorSpeed - 30) / 100.0;
        int16_t servoDegree = map(servoControl, -100, 100, -SERVO_RANGE, SERVO_RANGE);
        if (enableLock)
            motorSpeed = servoDegree = 0;

        int16_t leftServoDegree = servoDegree + storage.getLeftServoDelta(),
                rightServoDegree = servoDegree + storage.getRightServoDelta();
        int8_t leftMotorSpeed = motorSpeed,
               rightMotorSpeed = motorSpeed;

        CONTROL_MSG controlMsg = {.leftServoDegree = leftServoDegree, .rightServoDegree = rightServoDegree, .leftMotorSpeed = leftMotorSpeed, .rightMotorSpeed = rightMotorSpeed};
        BOAT_MSG boatMsg = {0, 0, 0};

        OLED::STATUS status = {
            .leftServoDegree = leftServoDegree,
            .rightServoDegree = rightServoDegree,
            .leftMotorSpeed = leftMotorSpeed,
            .rightMotorSpeed = rightMotorSpeed,
            .networkStatus = network.proceedClient(controlMsg, boatMsg),
        };
        if (status.networkStatus == RADIOLIB_ERR_NONE)
        {
            status.batteryVoltage = boatMsg.batteryVoltage;
            status.batteryPercentage = boatMsg.batteryPercentage;
            status.lastMsgTime = millis();
            status.mpuX = boatMsg.mpuX, status.mpuY = boatMsg.mpuY, status.mpuZ = boatMsg.mpuZ;
        }
        oled.updateStatus(status);
    }
}

void updateData(uint8_t &speedCruise,
                int8_t &speedControl, int8_t &steerControl,
                bool &enableCruise, bool &enableLock)
{
    if (enableCruise)
        speedControl = speedCruise;
    if (enableLock)
        speedControl = steerControl = 0;

    if (speedControl < 0)
        speedControl *= BACKWARD_LIMIT;
    else
        speedControl *= storage.getMaxSpeed() / 100.0;

    steerControl *= 60.0 / SERVO_RANGE;
    if (speedControl > 0)
        steerControl *= 1.0 - (speedControl - 30) / 100.0;
    steerControl = map(steerControl, -100, 100, -SERVO_RANGE, SERVO_RANGE);
}

#endif
