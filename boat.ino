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

// #define DEBUG
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
SERVO servo0(PIN_SERVO0, 100, SERVO_RANGE, 5, 25);
SERVO servo1(PIN_SERVO1, 100, SERVO_RANGE, 5, 25);
MOTOR motor0(PIN_MOTOR0, PIN_DIR0, 5000, 8, 2, false);
MOTOR motor1(PIN_MOTOR1, PIN_DIR1, 5000, 8, 2, true);
BATTERY battery(INA_ADDRESS, 14.8, 16.8);
NETWORK<CONTROL_MSG, BOAT_MSG> network(PIN_CS, PIN_IRQ, PIN_RESET, PIN_BUSY);
MPU6050 mpu6050(MPU_ADDRESS, MPU6050_RANGE_2_G, MPU6050_RANGE_250_DEG);

void setup()
{
    Serial.begin(115200);
    SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
    pinMode(PIN_DIR0, OUTPUT);
    pinMode(PIN_DIR1, OUTPUT);

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
                servo0.setAngle(controlMsg.servoDegree), servo1.setAngle(controlMsg.servoDegree);
                motor0.setSpeed(controlMsg.motorSpeed), motor1.setSpeed(controlMsg.motorSpeed);
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

    servo0.begin();
    servo1.begin();

    if (!motor0.begin())
    {
        Serial.println("Motor initialization failed");
        while (1)
            ;
    }

    if (!motor1.begin())
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

#define PIN_DEBUG 4
    pinMode(PIN_DEBUG, OUTPUT);
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
        Serial.println(lst_msg == 0 ? "No data received" : "Timeout");
        lst_chk = clock();
    }

    float_t ax;
    float_t ay;
    float_t az;
    float_t gx;
    float_t gy;
    float_t gz;
    mpu6050.readData(ax, ay, az, gx, gy, gz);
#ifdef DEBUG
    Serial.print("ax: ");
    Serial.print(ax);
    Serial.print(" ay: ");
    Serial.print(ay);
    Serial.print(" az: ");
    Serial.print(az);
    Serial.print(" gx: ");
    Serial.print(gx);
    Serial.print(" gy: ");
    Serial.print(gy);
    Serial.print(" gz: ");
    Serial.println(gz);
#endif

    digitalWrite(PIN_DEBUG, HIGH);
    ets_delay_us(1);
    digitalWrite(PIN_DEBUG, LOW);
}
#endif
