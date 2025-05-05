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

// This file should be empty. It is used due to
// an Arduino VSCode extension bug that requires
// a .ino file with the same name as the project folder.
// See: https://github.com/microsoft/vscode-arduino/issues/1665#issuecomment-1878109370

#if true

#include <RadioLib.h>
#include <define.hpp>

// Define pins for SX1281
#define RADIOLIB_CS 9
#define RADIOLIB_RESET 8
#define RADIOLIB_IRQ 18
#define RADIOLIB_BUSY 3

SX1281 radio = new Module(RADIOLIB_CS, RADIOLIB_RESET, RADIOLIB_IRQ, RADIOLIB_BUSY);

volatile bool irqFired = false;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("Device A - Sender");

    // Initialize SPI
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CSN);

    // Initialize radio
    if (radio.begin() != RADIOLIB_ERR_NONE)
    {
        Serial.println("Failed to initialize radio");
        while (1)
            ;
    }

    // Set frequency and other parameters
    radio.setFrequency(868.0);
    radio.setBandwidth(125.0);
    // ... Add other settings as needed

    // Enable CRC
    radio.setCRC(2, 0x1D0F, 0x1021);

    // Set interrupt action
    radio.setDio1Action(setFlag);
}

void loop()
{
    static unsigned long lastSend = 0;
    if (millis() - lastSend >= 100)
    { // Send CONTROL_MSG every 5 seconds
        lastSend = millis();
        sendControlMsg();
        // After sending, start receiving for BOAT_MSG
        radio.startReceive();
    }

    if (irqFired)
    {
        irqFired = false;
        // Check if a packet was received
        uint32_t irqStatus = radio.getIrqStatus();
        if (irqStatus & RADIOLIB_SX128X_IRQ_RX_DONE)
        {
            unsigned long data = 0;
            size_t len = sizeof(data);
            int state = radio.readData((uint8_t *)&data, len);
            if (state == RADIOLIB_ERR_NONE)
            {
                Serial.print("Received BOAT_MSG: ");
                Serial.println(data);
            }
            else
            {
                Serial.print("Failed to read data, code: ");
                Serial.println(state);
            }
            // Start receiving again
            radio.startReceive();
        }
    }
}

void setFlag(void)
{
    irqFired = true;
}

void sendControlMsg()
{
    unsigned long controlMsg = millis();
    int state = radio.transmit((uint8_t *)&controlMsg, sizeof(controlMsg));
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("CONTROL_MSG sent");
    }
    else
    {
        Serial.print("Failed to send CONTROL_MSG, code: ");
        Serial.println(state);
    }
}

#else

#include <RadioLib.h>
#include <define.hpp>

// Define pins for SX1281
#define RADIOLIB_CS 9
#define RADIOLIB_RESET 8
#define RADIOLIB_IRQ 18
#define RADIOLIB_BUSY 3 // Adjust if necessary

SX1281 radio = new Module(RADIOLIB_CS, RADIOLIB_RESET, RADIOLIB_IRQ, RADIOLIB_BUSY);

volatile bool irqFired = false;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("Device B - Receiver");

    // Initialize SPI
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CSN);

    // Initialize radio
    if (radio.begin() != RADIOLIB_ERR_NONE)
    {
        Serial.println("Failed to initialize radio");
        while (1)
            ;
    }

    // Set frequency and other parameters (must match Device A)
    radio.setFrequency(868.0);
    radio.setBandwidth(125.0);
    // ... Add other settings as needed

    // Enable CRC
    radio.setCRC(2, 0x1D0F, 0x1021);

    // Set interrupt action
    radio.setDio1Action(setFlag);

    // Start receiving
    radio.startReceive();
}

void loop()
{
    if (irqFired)
    {
        irqFired = false;
        // Check if a packet was received
        uint32_t irqStatus = radio.getIrqStatus();
        if (irqStatus & RADIOLIB_SX128X_IRQ_RX_DONE)
        {
            unsigned long data;
            size_t len = sizeof(data);
            int state = radio.readData((uint8_t *)&data, len);
            if (state == RADIOLIB_ERR_NONE)
            {
                Serial.print("Received CONTROL_MSG: ");
                Serial.println(data);
                // Run callback
                doSomething();
                // Send BOAT_MSG as response
                sendBoatMsg();
                // Start receiving again
                radio.startReceive();
            }
        }
    }
}

void setFlag(void)
{
    irqFired = true;
}

void doSomething()
{
    // Serial.println("Doing something after receiving CONTROL_MSG");
    // User-defined callback action
}

void sendBoatMsg()
{
    unsigned long boatMsg = millis();
    int state = radio.transmit((uint8_t *)&boatMsg, sizeof(boatMsg));
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("BOAT_MSG sent");
    }
    else
    {
        Serial.print("Failed to send BOAT_MSG, code: ");
        Serial.println(state);
    }
}

#endif
