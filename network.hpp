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

#include <RF24.h>

template <typename CLIENT_MSG, typename SERVER_MSG>
class NETWORK
{
private:
    RF24 radio;
    uint8_t channel;
    rf24_datarate_e dataRate;
    rf24_pa_dbm_e powerLevel;
    uint8_t retriesDelay;
    uint8_t retriesCount;
    uint64_t address;
    bool isServer = false;
    uint8_t pipe;
    std::function<SERVER_MSG(CLIENT_MSG)> serverCallback;

public:
    NETWORK(uint8_t pinCE, uint8_t pinCSN,
            uint8_t channel, rf24_datarate_e dataRate, rf24_pa_dbm_e powerLevel,
            uint8_t retriesDelay, uint8_t retriesCount,
            uint64_t address);
    bool begin();
    void setServer(uint8_t pipe, std::function<SERVER_MSG(CLIENT_MSG)> serverCallback);
    void setClient();
    bool proceedServer(boolean &hasData);
    bool proceedClient(CLIENT_MSG clientMsg, SERVER_MSG &serverMsg);
};
