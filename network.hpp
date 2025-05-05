// Copyright (C) 2025 Langning Chen
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

#pragma once

#include <RadioLib.h>
#include <functional>
#include <cstring>

template <typename CLIENT_MSG, typename SERVER_MSG>
class NETWORK
{
private:
    SX1281 *radio;
    float frequency;
    float bandwidth;
    uint8_t spreadingFactor;
    float codingRate;
    int8_t outputPower;
    uint32_t syncWord;
    bool isServer = false;
    std::function<SERVER_MSG(CLIENT_MSG)> serverCallback;
    volatile bool packetReceived = false;

    static void onReceive(void *context)
    {
        ((NETWORK *)context)->packetReceived = true;
    }

public:
    NETWORK(uint8_t cs, uint8_t reset, uint8_t irq, uint8_t busy = RADIOLIB_NC,
            float frequency = 868.0, float bandwidth = 125.0, uint8_t spreadingFactor = 7,
            float codingRate = 5.0, int8_t outputPower = 13, uint32_t syncWord = 0x1424)
        : frequency(frequency), bandwidth(bandwidth), spreadingFactor(spreadingFactor),
          codingRate(codingRate), outputPower(outputPower), syncWord(syncWord)
    {
        radio = new SX1281(cs, reset, irq, busy);
    }

    ~NETWORK()
    {
        delete radio;
    }

    bool begin()
    {
        if (!radio->begin())
            return false;
        radio->setFrequency(frequency);
        radio->setBandwidth(bandwidth);
        radio->setSpreadingFactor(spreadingFactor);
        radio->setCodingRate(codingRate);
        radio->setOutputPower(outputPower);
        radio->setSyncWord(syncWord);
        radio->setCRC(2); // 16-bit CRC
        return true;
    }

    void setServer(std::function<SERVER_MSG(CLIENT_MSG)> serverCallback)
    {
        this->serverCallback = serverCallback;
        radio->setDio1Action(onReceive, this);
        radio->startReceive();
        isServer = true;
    }

    void setClient()
    {
        isServer = false;
    }

    bool proceedServer(bool &hasData)
    {
        if (packetReceived)
        {
            packetReceived = false;
            CLIENT_MSG clientMsg;
            memset(&clientMsg, 0, sizeof(clientMsg));
            size_t len = sizeof(clientMsg);
            int state = radio->readData((uint8_t *)&clientMsg, len);
            if (state == RADIOLIB_ERR_NONE)
            {
                hasData = true;
                SERVER_MSG serverMsg = serverCallback(clientMsg);
                radio->transmit((uint8_t *)&serverMsg, sizeof(serverMsg));
            }
            else
            {
                hasData = false;
            }
            uint32_t irqFlags = radio->getIrqFlags();
            radio->clearIrqStatus(irqFlags);
            radio->startReceive();
        }
        else
        {
            hasData = false;
        }
        return true;
    }

    bool proceedClient(CLIENT_MSG clientMsg, SERVER_MSG &serverMsg)
    {
        int state = radio->transmit((uint8_t *)&clientMsg, sizeof(clientMsg));
        if (state != RADIOLIB_ERR_NONE)
            return false;
        memset(&serverMsg, 0, sizeof(serverMsg));
        size_t len = sizeof(serverMsg);
        state = radio->receive((uint8_t *)&serverMsg, len, 1000); // 1 second timeout
        if (state == RADIOLIB_ERR_NONE)
        {
            return true;
        }
        return false;
    }
};
