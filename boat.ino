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

SERVO servo0(PIN_SERVO0, 100, 12, SERVO_RANGE, 5, 25);
SERVO servo1(PIN_SERVO1, 100, 12, SERVO_RANGE, 5, 25);
MOTOR motor0(PIN_MOTOR0, PIN_DIR0, 5000, 8, false);
MOTOR motor1(PIN_MOTOR1, PIN_DIR1, 5000, 8, false);
BATTERY battery(INA_ADDRESS, 14.8, 16.8);
NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_IRQ, PIN_RESET, PIN_BUSY);
MPU6050 mpu6050(MPU_ADDRESS, MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG);
BOAT_MSG boatInitMsg;

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);

    boatInitMsg.type = BOAT_MSG::BOAT_INIT_MSG;
    boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::SUCCESS;
    strcpy(boatInitMsg.initMsg.compileDatetime, __DATE__ " " __TIME__);
    pinMode(PIN_DIR0, OUTPUT);
    pinMode(PIN_DIR1, OUTPUT);

    if (!network.begin())
    {
        Serial.println("Network initialization failed");
        while (1)
            ;
    }

    if (!network.setServer(
            [&](CONTROL_MSG controlMsg) -> BOAT_MSG
            {
                static bool firstSent = false;
                if (!firstSent)
                {
                    firstSent = true;
                    return boatInitMsg;
                }
                servo0.setAngle(controlMsg.leftServoDegree), servo1.setAngle(controlMsg.rightServoDegree);
                motor0.setSpeed(controlMsg.leftMotorSpeed), motor1.setSpeed(controlMsg.rightMotorSpeed);
                BOAT_MSG boatMsg;
                boatMsg.type = BOAT_MSG::BOAT_STATUS_MSG;
                boatMsg.statusMsg.batteryVoltage = battery.getVoltage();
                boatMsg.statusMsg.batteryPercentage = battery.getPercentage();
                mpu6050.readData(boatMsg.statusMsg.mpuAX, boatMsg.statusMsg.mpuAY, boatMsg.statusMsg.mpuAZ,
                                 boatMsg.statusMsg.mpuGX, boatMsg.statusMsg.mpuGY, boatMsg.statusMsg.mpuGZ);
                return boatMsg;
            }))
    {
        Serial.println("Network server initialization failed");
        while (1)
            ;
    }

    if (!Wire.begin(PIN_SDA, PIN_SCL))
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::I2C_FAILED;
    if (!servo0.begin())
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::SERVO0_FAILED;
    if (!servo1.begin())
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::SERVO1_FAILED;
    if (!motor0.begin())
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::MOTOR0_FAILED;
    if (!motor1.begin())
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::MOTOR1_FAILED;
    if (!battery.begin())
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::BATTERY_FAILED;
    if (!mpu6050.begin())
        boatInitMsg.initMsg.status = BOAT_MSG::INIT_MSG::MPU6050_FAILED;

    Serial.println("Boat initialization completed");
}

clock_t lst_msg = 0;
clock_t lst_chk = 0;

void loop()
{
    bool hasData = false;
    int16_t status = network.proceedServer(hasData);
    if (status != RADIOLIB_ERR_NONE)
    {
        Serial.print("Network error: ");
        Serial.println(status);
    }
    if (hasData)
        lst_msg = clock();
    else if (clock() - lst_msg > CONTROL_TIMEOUT && clock() - lst_chk > 1000)
    {
        if (lst_msg != 0)
        {
            servo0.setAngle(0), servo1.setAngle(0);
            motor0.setSpeed(0), motor1.setSpeed(0);
        }
        lst_chk = clock();
    }
}
#endif
