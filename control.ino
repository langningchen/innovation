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

NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_RESET, PIN_IRQ, PIN_BUSY);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

    if (!network.begin())
    {
        Serial.println("Network initialization failed");
        while (1)
            ;
    }
    network.setClient();

    Serial.println("Control initialization completed");
}

void loop()
{
    if (Serial.available())
    {
        uint8_t servoDegree = Serial.readStringUntil(' ').toInt();
        uint8_t motorSpeed = Serial.readStringUntil('\n').toInt();
        CONTROL_MSG controlMsg = {servoDegree, motorSpeed};
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
            Serial.print("Network error");
    }
}

#endif
