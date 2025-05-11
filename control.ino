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

NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_IRQ, PIN_RESET, PIN_BUSY);
A2D a2d(PIN_SPEED_MAX, PIN_SPEED_CRUISE, PIN_SPEED_CONTROL, PIN_STEER_CONTROL, PIN_CRUISE_CONTROL, PIN_CONTROL_LOCK);
OLED oled(OLED_ADDRESS, OLED_WIDTH, OLED_HEIGHT);

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

    if (!a2d.begin())
    {
        Serial.println("A2D initialization failed");
        while (1)
            ;
    }
    a2d.collaborate();

    Serial.println("Control initialization completed");
}

uint32_t lastMsg = 0;

void loop()
{
    a2d.process();
    if (lastMsg == 0 || millis() - lastMsg > MSG_INTERVAL)
    {
        lastMsg += MSG_INTERVAL;
        uint8_t speedMax, speedCruise;
        int8_t speedControl, steerControl;
        bool cruiseControl, controlLock;
        a2d.getData(speedMax, speedCruise, speedControl, steerControl, cruiseControl, controlLock);
        CONTROL_MSG controlMsg = {(steerControl + 100) * 0.9, speedControl};
        BOAT_MSG boatMsg = {0, 0, 0};
        if (network.proceedClient(controlMsg, boatMsg))
        {
            Serial.print("servo ");
            Serial.print(controlMsg.servoDegree);
            Serial.print("°  motor ");
            Serial.print(controlMsg.motorSpeed);
            Serial.print("%  battery ");
            Serial.print(boatMsg.batteryVoltage);
            Serial.print("V ");
            Serial.print(boatMsg.batteryPercentage);
            Serial.println("%");
        }
        else
            Serial.println("Network error");
    }
}

#endif
