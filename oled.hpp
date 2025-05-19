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

#include <vector>
#include <string>
#include <Adafruit_SSD1306.h>
#include <storage.hpp>
#include <a2d.hpp>

class OLED
{
public:
    enum PAGE
    {
        PAGE_INIT,
        PAGE_CONFIG,
        PAGE_STATUS,
    } page;

    struct STATUS
    {
        float_t batteryVoltage;
        uint8_t batteryPercentage;
        int16_t leftServoDegree, rightServoDegree;
        int8_t leftMotorSpeed, rightMotorSpeed;
        int16_t networkStatus;
        uint32_t lastMsgTime;
        float_t mpuX, mpuY, mpuZ;
    };

    class MENU
    {
    private:
        enum TYPE
        {
            TEXT,
            CONFIG,
            FOLDER,
            BUTTON,
        };
        struct CONFIG_DATA
        {
            int32_t value;
            int32_t minValue;
            int32_t maxValue;
        };
        struct FOLDER_DATA
        {
            uint8_t index = 0;
            std::vector<MENU *> subMenu;
        };

        Adafruit_SSD1306 *display = nullptr;
        String name;
        MENU *parent = nullptr, *next = nullptr, *prev = nullptr;
        bool isConfigActive = false;
        std::function<void(MENU *)> onLoad;
        std::function<void(MENU *)> onBlur;
        std::function<void(MENU *)> onClick;
        TYPE type;
        CONFIG_DATA config;
        FOLDER_DATA folder;

        void load();
        void blur();
        void click();

    public:
        MENU(String name);
        MENU(String name, std::initializer_list<MENU *> subMenu);
        MENU(String name, std::function<void(MENU *)> onLoad, std::function<void(MENU *)> onBlur,
             int32_t minValue, int32_t maxValue);
        MENU(String name, std::function<void(MENU *)> onClick);
        ~MENU();
        void render();
        void updateDisplay();
        void setDisplay(Adafruit_SSD1306 *display);

        friend class OLED;
    };

private:
    uint8_t address;
    uint8_t width, height;
    Adafruit_SSD1306 display;
    STATUS status;
    MENU *menu, *currentMenu;
    bool needUpdate;
    STORAGE &storage;

    void setColor(bool black);

    void renderInit();
    void renderConfig();
    void renderStatus();

public:
    OLED(uint8_t address, uint8_t width, uint8_t height, STORAGE &storage, A2D &a2d);
    ~OLED();
    bool begin();
    void process();
    PAGE getPage();
    void switchPage(PAGE page);

    void dirInput(DIR dir);
    void knobInput(uint8_t value);
    void updateStatus(STATUS status);
};
