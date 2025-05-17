#include <oled.hpp>
#include <define.hpp>

/**
 * @brief The menu is loaded
 */
void OLED::MENU::load()
{
    if (onLoad)
        onLoad(this);
}

/**
 * @brief The menu is blurred
 */
void OLED::MENU::blur()
{
    if (onBlur)
        onBlur(this);
}

/**
 * @brief The menu is clicked
 */
void OLED::MENU::click()
{
    if (onClick)
        onClick(this);
}

/**
 * @brief MENU (text) constructor
 * @param name Name of the menu
 */
OLED::MENU::MENU(String name) : type(TYPE::TEXT), name(name) {}

/**
 * @brief MENU (folder) constructor
 * @param name Name of the menu
 * @param subMenu The menu items
 */
OLED::MENU::MENU(String name, std::initializer_list<MENU *> subMenu) : type(TYPE::FOLDER), name(name)
{
    for (MENU *menu : subMenu)
    {
        if (type != TYPE::FOLDER)
            return;
        if (folder.subMenu.size() > 0)
        {
            folder.subMenu.back()->next = menu;
            menu->prev = folder.subMenu.back();
        }
        menu->parent = this;
        folder.subMenu.push_back(menu);
    }
    onLoad = [this](MENU *menu)
    {
        for (MENU *item : folder.subMenu)
            if (item->type == TYPE::CONFIG && item->onLoad)
                item->onLoad(item);
    };
}

/**
 * @brief MENU (config) constructor
 * @param name Name of the menu
 * @param onLoad callback function, it will be called when the item is showed
 * @param onBlur callback function, it will be called when the item is blurred
 */
OLED::MENU::MENU(String name, std::function<void(MENU *)> onLoad, std::function<void(MENU *)> onBlur)
    : type(TYPE::CONFIG), name(name), onLoad(onLoad), onBlur(onBlur) {}

/**
 * @brief MENU (button) constructor
 * @param name Name of the menu
 */
OLED::MENU::MENU(String name, std::function<void(MENU *)> onClick)
    : type(TYPE::BUTTON), name(name), onClick(onClick) {}

/**
 * @brief MENU destructor
 */
OLED::MENU::~MENU()
{
    if (type == TYPE::FOLDER)
        for (MENU *item : folder.subMenu)
            delete item;
}

/**
 * @brief Render the configuration menu
 */
void OLED::MENU::render()
{
    updateDisplay();
    display->setCursor(0, 0);
    display->setTextSize(1);
    uint8_t page = folder.index / OLED_LINE_CNT;
    for (uint8_t i = OLED_LINE_CNT * page;
         i < OLED_LINE_CNT * (page + 1) && i < folder.subMenu.size();
         i++)
    {
        MENU *item = folder.subMenu[i];
        if (i == folder.index && !isConfigActive)
            display->setTextColor(BLACK, WHITE);
        else
            display->setTextColor(WHITE, BLACK);
        String leftText = item->name;
        String rightText = "";
        const int16_t width = display->width() / OLED_CHAR_WIDTH;
        if (item->type == TYPE::CONFIG)
            rightText = item->config.value;
        if (leftText.length() + rightText.length() > width)
            display->print(leftText.substring(0, width - rightText.length() - 3) + "...");
        else
            display->print(leftText);
        for (int16_t i = leftText.length() + rightText.length(); i < width; i++)
            display->print(' ');
        if (i == folder.index && isConfigActive)
            display->setTextColor(BLACK, WHITE);
        display->println(rightText);
    }
}

/**
 * @brief Update the display object
 */
void OLED::MENU::updateDisplay()
{
    if (!display)
    {
        MENU *pointer = this;
        while (pointer && pointer->display == nullptr)
            pointer = pointer->parent;
        if (pointer)
            display = pointer->display;
    }
}

/**
 * @brief Set the display object
 * @param display The SSD1306 display object
 */
void OLED::MENU::setDisplay(Adafruit_SSD1306 *display) { this->display = display; }

/**
 * @brief Render the initialization page
 */
void OLED::renderInit()
{
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.println("Innovation");
    display.setTextColor(WHITE, BLACK);
    display.println();
    display.println("Compile time");
    display.println(__DATE__);
    display.println(__TIME__);
}

/**
 * @brief Render the configuration menu
 */
void OLED::renderConfig() { currentMenu->render(); }

/**
 * @brief Render the status page
 */
void OLED::renderStatus()
{
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);
    display.drawRoundRect(0, 0, 128, 64, 5, WHITE);
    display.setCursor(4, 4), display.print(status.networkStatus ? String(status.networkStatus) : "OK");
    display.setCursor(64, 4), display.print(millis() - status.lastMsgTime);

    display.setCursor(4, 4 + OLED_CHAR_HEIGHT), display.print(String(status.servoDegree) + "deg");
    display.setCursor(64, 4 + OLED_CHAR_HEIGHT), display.print(String(status.motorSpeed) + "%");

    display.setCursor(4, 4 + 2 * OLED_CHAR_HEIGHT), display.print(String(status.batteryVoltage) + "V");
    display.setCursor(64, 4 + 2 * OLED_CHAR_HEIGHT), display.print(String(status.batteryPercentage) + "%");

    display.setCursor(4, 4 + 3 * OLED_CHAR_HEIGHT), display.print(String(status.mpuX));
    display.setCursor(44, 4 + 3 * OLED_CHAR_HEIGHT), display.print(String(status.mpuY));
    display.setCursor(84, 4 + 3 * OLED_CHAR_HEIGHT), display.print(String(status.mpuZ));
}

/**
 * @brief OLED constructor
 * @param address I2C address of the OLED display
 * @param width Width of the display
 * @param height Height of the display
 */
OLED::OLED(uint8_t address, uint8_t width, uint8_t height, STORAGE &storage)
    : address(address), width(width), height(height),
      needUpdate(true), display(width, height, &Wire, -1)
{
    currentMenu = menu =
        new MENU("Main Menu",
                 {
                     new MENU("Boat", {
                                          new MENU("Motor", {
                                                                new MENU("Max speed", [&storage](MENU *menu)
                                                                         {
                                                                             Serial.println("max speed get");
                                                                               menu->config.value = storage.getMaxSpeed(); }, [&storage](MENU *menu)
                                                                         {
                                                                             Serial.println("max speed set to " + String(menu->config.value));
                                                                              Serial.println(storage.setMaxSpeed(menu->config.value)); }),
                                                            }),
                                      }),
                     new MENU("About", {
                                           new MENU("https://github.com"),
                                           new MENU("/langningchen"),
                                           new MENU("/innovation"),
                                           new MENU(""),
                                           new MENU("GNU GPLv3"),
                                       }),
                 });
    menu->setDisplay(&display);
}

/**
 * @brief OLED destructor
 */
OLED::~OLED()
{
    delete menu;
    menu = currentMenu = nullptr;
}

/**
 * @brief Initialize the OLED display
 * @return true if successful, false otherwise
 * @details This function will initialize the OLED display
 * and print the compile time and date
 */
bool OLED::begin()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, address))
        return false;
    process();
    return true;
}

/**
 * @brief Process the OLED display
 * @details This function will update the OLED display
 * with the current page and display the information
 */
void OLED::process()
{
    if (!needUpdate)
        return;
    display.clearDisplay();
    switch (page)
    {
    case PAGE_INIT:
        renderInit();
        break;
    case PAGE_CONFIG:
        renderConfig();
        break;
    case PAGE_STATUS:
        renderStatus();
        break;
    default:
        break;
    }
    display.display();
    needUpdate = false;
}

/**
 * @brief Get the current page
 * @return The current page
 */
OLED::PAGE OLED::getPage() { return page; }

/**
 * @brief Set the current page
 * @param page The page to set
 */
void OLED::switchPage(PAGE page)
{
    if (this->page == page)
        return;
    this->page = page;
    needUpdate = true;
}

/**
 * @brief Handle direction input
 * @param dir The direction input
 */
void OLED::dirInput(DIR dir)
{
    if (page != PAGE::PAGE_CONFIG)
        return;
    uint8_t &index = currentMenu->folder.index;
    std::vector<OLED::MENU *> &subMenu = currentMenu->folder.subMenu;
    MENU *oldMenu = currentMenu;
    switch (dir)
    {
    case DIR::UP:
        if (needUpdate = !currentMenu->isConfigActive)
            index = (index + subMenu.size() - 1) % subMenu.size();
        break;
    case DIR::DOWN:
        if (needUpdate = !currentMenu->isConfigActive)
            index = (index + 1) % subMenu.size();
        break;
    case DIR::LEFT:
        if (needUpdate = currentMenu->parent)
            if (currentMenu->isConfigActive)
                currentMenu->isConfigActive = false, subMenu[index]->blur();
            else
                currentMenu = currentMenu->parent;
        break;
    case DIR::RIGHT:
        if (needUpdate = subMenu[index]->type == MENU::TYPE::FOLDER)
            currentMenu = subMenu[index], currentMenu->load();
        else if (needUpdate = subMenu[index]->type == MENU::TYPE::CONFIG && !currentMenu->isConfigActive)
            currentMenu->isConfigActive = true;
        else if (needUpdate = subMenu[index]->type == MENU::TYPE::BUTTON)
            subMenu[index]->click();
        break;
    default:
        break;
    }
}

/**
 * @brief Handle knob input
 */
void OLED::knobInput(uint8_t value)
{
    value = constrain(value, 0, 100);
    if (page != PAGE::PAGE_CONFIG)
        return;
    if (!currentMenu->folder.subMenu.size())
        return;
    MENU *modifingMenu = currentMenu->folder.subMenu[currentMenu->folder.index];
    if (modifingMenu->type != MENU::TYPE::CONFIG || !currentMenu->isConfigActive)
        return;
    modifingMenu->config.value = map(value, 0, 100, modifingMenu->config.minValue, modifingMenu->config.maxValue);
    needUpdate = true;
}

/**
 * @brief Update the status
 * @param status The status to update
 */
void OLED::updateStatus(STATUS status) { this->status = status, needUpdate = true; }
