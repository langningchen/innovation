<!--
 Copyright (C) 2025 langningchen
 
 This file is part of innovation.
 
 innovation is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 innovation is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with innovation.  If not, see <https://www.gnu.org/licenses/>.
-->

# Innovation for the Future

[![wakatime](https://wakatime.com/badge/user/576a8b82-13f2-4f92-aa69-6fff06354a54/project/70d26c7c-18f7-4863-a7f6-ae7049ec9f53.svg)](https://wakatime.com/badge/user/576a8b82-13f2-4f92-aa69-6fff06354a54/project/70d26c7c-18f7-4863-a7f6-ae7049ec9f53)

The school has an event called "Innovation for the Future", which requires each group to design an innovative ship model. The ship model should be able to sail on water and can be made of any material. The design should be creative and practical, and the ship model should be able to carry a certain amount of weight. The event encourages students to think outside the box and come up with unique ideas for their ship models.

## Network communication

The ship model should be able to communicate with a controller via a 2.4G wireless network. 

#### Network Communication Architecture
```mermaid
sequenceDiagram
    participant Control
    participant Boat

    Note over Control,Boat: 2.4G wireless (SX1281, RadioLib)
    Control -->> Boat: CONTROL_MSG
    Control -->> Boat: CONTROL_MSG
    Note over Boat: Boat initialized
    Control ->> Boat: CONTROL_MSG
    Boat ->> Control: BOAT_MSG (type: BOAT_INIT_MSG)
    Control ->> Boat: CONTROL_MSG
    Boat ->> Control: BOAT_MSG (type: BOAT_STATUS_MSG)
```

#### Network Message Structure
```mermaid
classDiagram
    class CONTROL_MSG {
        type: CONTROL_NOP | CONTROL_CONFIG_MSG | CONTROL_COMMAND_MSG
    }

    class CONFIG_MSG {
        bool motor0Direction, motor1Direction
        uint8_t controlTimeout
    }

    class COMMAND_MSG {
        int16_t leftServoDegree
        int16_t rightServoDegree
        int8_t leftMotorSpeed
        int8_t rightMotorSpeed
    }

    CONTROL_MSG --> CONFIG_MSG
    CONTROL_MSG --> COMMAND_MSG

    class BOAT_MSG {
        type: BOAT_NOP | BOAT_INIT_MSG | BOAT_STATUS_MSG
    }

    class INIT_MSG {
        char compileDatetime[32]
        status: SUCCESS | I2C_FAILED | SERVO0_FAILED | ...
    }

    class STATUS_MSG {
        float_t batteryVoltage
        uint8_t batteryPercentage
        float_t mpuAX, mpuAY, mpuAZ
        float_t mpuGX, mpuGY, mpuGZ
    }
    BOAT_MSG --> INIT_MSG
    BOAT_MSG --> STATUS_MSG
```

## Project Structure

```mermaid
graph TD
    A[switch, define<br>Config & Utilities]

    subgraph Controller
        CA[control<br>Controller Main Program]
        CB[network<br>Network Communication]
        CC[storage<br>Long-term Storage]
        CD[a2d<br>Joystick and Knob Control]
        CE[oled<br>Display]
        CF[messages<br>Message Definitions]
    end

    CA --> CB
    CA --> CC
    CA --> CD
    CA --> CE
    CD --> CC
    CE --> CC
    CE --> CD
    CB --> CF
    CE --> CF
    Controller --> A
    
    subgraph Boat
        BA[boat<br>Boat Main Program]
        BB[servo<br>Servo Control]
        BC[motor<br>Motor Control]
        BD[battery<br>Battery Monitor]
        BE[network<br>Network Communication]
        BF[mpu6050<br>MPU6050 Sensor]
        BG[pwm<br>PWM Control]
    end

    BA --> BB
    BA --> BC
    BA --> BD
    BA --> BE
    BA --> BF
    BB --> BG
    BC --> BG
    Boat --> A
```
