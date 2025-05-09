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
#ifdef BOAT

#include <define.hpp>
#include <servo.hpp>
#include <motor.hpp>
#include <battery.hpp>
#include <network.hpp>
#include <messages.hpp>
#include <mpu6050.hpp>

// CAUTION! It seems that setting
// any channel to 0 will cause
// the configuration of first PWM channel
// overwritten by the second one
SERVO servo(PIN_PWM0, 100, 12, 1, 180, 5, 25);
MOTOR motor(PIN_PWM1, 5000, 8, 2);
BATTERY battery(PIN_ADC,
                14.0 / (270 + 40) * 40,  // 1.8064516129
                14.8 / (270 + 40) * 40); // 1.9096774194
NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_RESET, PIN_IRQ, PIN_BUSY);
MPU6050 mpu6050(MPU_ADDRESS, MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

    if (!Wire.begin(PIN_SDA, PIN_SCL))
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

    if (!network.setServer(
            [&](CONTROL_MSG controlMsg) -> BOAT_MSG
            {
                servo.setAngle(controlMsg.servoDegree);
                motor.setSpeed(controlMsg.motorSpeed);
                BOAT_MSG boatMsg;
                boatMsg.result = 0;
                boatMsg.batteryVoltage = battery.getVoltage();
                boatMsg.batteryPercentage = battery.getPercentage();
                Serial.print("servo ");
                Serial.print(controlMsg.servoDegree);
                Serial.print("°  motor ");
                Serial.print(controlMsg.motorSpeed);
                Serial.print("%  battery ");
                Serial.print(boatMsg.batteryVoltage);
                Serial.print("V ");
                Serial.print(boatMsg.batteryPercentage);
                Serial.println("%");
                return boatMsg;
            }))
    {
        Serial.println("Network server initialization failed");
        while (1)
            ;
    }

    if (!servo.begin())
    {
        Serial.println("Servo initialization failed");
        while (1)
            ;
    }

    if (!motor.begin())
    {
        Serial.println("Motor initialization failed");
        while (1)
            ;
    }

    if (!battery.begin())
    {
        Serial.println("Battery initialization failed");
        while (1)
            ;
    }

    if (!mpu6050.begin())
    {
        Serial.println("MPU6050 initialization failed");
        while (1)
            ;
    }

    Serial.println("Boat initialization completed");
}

clock_t lst_msg = 0;
clock_t lst_chk = 0;

void loop()
{
    bool hasData = false;
    if (!network.proceedServer(hasData))
    {
        Serial.println("Network error");
        return;
    }
    if (hasData)
        lst_msg = clock();
    else if (clock() - lst_msg > CONTROL_TIMEOUT && clock() - lst_chk > 1000)
    {
        Serial.println(lst_msg == 0 ? "No data received" : "Timeout");
        lst_chk = clock();
    }
}

#endif
