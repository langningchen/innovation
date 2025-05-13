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
        Adafruit_SSD1306 *display;
        String name;
        uint8_t index;
        MENU *parent, *next, *prev;
        std::vector<MENU *> subMenu;

    public:
        MENU(Adafruit_SSD1306 *display, String name);
        ~MENU();
        void addSubMenu(MENU *menu);
        void render();

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

    void configUp();
    void configDown();
    void configBack();
    void configEnter();
};
