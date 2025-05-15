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

class OLED
{
public:
    enum PAGE
    {
        INIT,
        CONFIG,
        STATUS,
    } page;

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
            uint8_t value = 0;
            uint8_t minValue = 0;
            uint8_t maxValue = 100;
            uint8_t defaultValue = 0;
        };
        struct FOLDER_DATA
        {
            uint8_t index = 0;
            std::vector<MENU *> subMenu;
        };

        Adafruit_SSD1306 *display = nullptr;
        String name;
        MENU *parent = nullptr, *next = nullptr, *prev = nullptr;
        std::function<void(MENU *)> onFocus;
        std::function<void(MENU *)> onBlur;
        std::function<void(MENU *)> onClick;
        TYPE type;
        CONFIG_DATA config;
        FOLDER_DATA folder;

        void focus();
        void blur();
        void click();

    public:
        MENU(String name);
        MENU(String name, std::initializer_list<MENU *> subMenu);
        MENU(String name, std::function<void(MENU *)> onFocus, std::function<void(MENU *)> onBlur);
        MENU(String name, std::function<void(MENU *)> onClick);
        ~MENU();
        void render();
        void setDisplay(Adafruit_SSD1306 *display);

        friend class OLED;
    };

private:
    uint8_t address;
    uint8_t width, height;
    Adafruit_SSD1306 display;
    MENU *menu, *currentMenu;
    bool needUpdate;

    void renderInit();
    void renderConfig();
    void renderStatus();

public:
    OLED(uint8_t address, uint8_t width, uint8_t height);
    ~OLED();
    bool begin();
    void process();
    PAGE getPage();
    void switchPage(PAGE page);

    void dirInput(DIR dir);
    void knobInput(uint8_t value);
};
