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

template <typename CLIENT_MSG, typename SERVER_MSG>
class NETWORK
{
private:
    SX1281 radio;
    bool packageReceived = false;
    bool isServer = false;
    std::function<SERVER_MSG(CLIENT_MSG)> serverCallback;

public:
    NETWORK(uint8_t pinCS, uint8_t pinRESET, uint8_t pinIRQ, uint8_t pinBUSY);
    bool begin();
    bool setServer(std::function<SERVER_MSG(CLIENT_MSG)> serverCallback);
    void setClient();
    bool proceedServer(boolean &hasData);
    bool proceedClient(CLIENT_MSG clientMsg, SERVER_MSG &serverMsg);
};
