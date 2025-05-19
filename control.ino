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
#include <oled.hpp>

NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_IRQ, PIN_RESET, PIN_BUSY);
STORAGE storage;
A2D a2d(PIN_SPEED_MAX, PIN_SPEED_CRUISE, PIN_SPEED_CONTROL, PIN_STEER_CONTROL, PIN_CRUISE_CONTROL, PIN_CONTROL_LOCK, storage);
OLED oled(OLED_ADDRESS, OLED_WIDTH, OLED_HEIGHT, storage, a2d);

uint32_t lastMsg;
uint8_t config, speedCruise;
int8_t motorControl, servoControl;
bool enableCruise, enableLock;
OLED::STATUS status;

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

    if (!oled.begin())
    {
        Serial.println("OLED initialization failed");
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

    storage.reset();
}

void loop()
{
    a2d.process(false);
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

        if (enableCruise)
            motorControl = speedCruise;
        int8_t motorSpeed = motorControl * (motorControl < 0 ? storage.getBackwardLimit() : storage.getMaxSpeed()) / 100;
        servoControl = servoControl * storage.getServoLimit() * 1.0 / SERVO_RANGE;
        if (motorSpeed > 30)
            servoControl *= 1.0 - (motorSpeed - 30) / 100.0;
        int16_t servoDegree = map(servoControl, -100, 100, -SERVO_RANGE, SERVO_RANGE);
        if (enableLock)
            motorSpeed = servoDegree = 0;

        int16_t leftServoDegree = servoDegree + storage.getLeftServoDelta(),
                rightServoDegree = servoDegree + storage.getRightServoDelta();
        int8_t leftMotorSpeed = motorSpeed + storage.getLeftMotorDelta(),
               rightMotorSpeed = motorSpeed + storage.getRightMotorDelta();

        CONTROL_MSG controlMsg = {.leftServoDegree = leftServoDegree, .rightServoDegree = rightServoDegree, .leftMotorSpeed = leftMotorSpeed, .rightMotorSpeed = rightMotorSpeed};
        BOAT_MSG boatMsg = {0, 0, 0};

        status.leftServoDegree = leftServoDegree;
        status.rightServoDegree = rightServoDegree;
        status.leftMotorSpeed = leftMotorSpeed;
        status.rightMotorSpeed = rightMotorSpeed;
        status.networkStatus = network.proceedClient(controlMsg, boatMsg);
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

#endif
