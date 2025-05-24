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
 * @param minValue Minimum value of the configuration
 * @param maxValue Maximum value of the configuration
 */
OLED::MENU::MENU(String name, std::function<void(MENU *)> onLoad, std::function<void(MENU *)> onBlur,
                 int32_t minValue, int32_t maxValue)
    : type(TYPE::CONFIG), name(name), onLoad(onLoad), onBlur(onBlur)
{
    config.minValue = minValue;
    config.maxValue = maxValue;
}

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
        else if (item->type == TYPE::FOLDER)
            rightText = ">";
        else if (item->type == TYPE::BUTTON)
            rightText = "=";
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
 * @brief Set the color of the OLED display
 * @param black If true, set the color to black, otherwise set it to white
 */
void OLED::setColor(bool black)
{
    if (black)
        display.setTextColor(BLACK, WHITE);
    else
        display.setTextColor(WHITE, BLACK);
}

/**
 * @brief Render the initialization page
 */
void OLED::renderInit()
{
    display.setCursor(4, 20);
    display.setTextSize(2);
    setColor(true);
    display.println("Innovation");
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
    uint32_t timeDelta = millis() - status.lastMsgTime;
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.drawRoundRect(0, 0, 128, 64, 5, WHITE);
    display.setCursor(4, 4),
        setColor(status.networkStatus),
        display.print(status.networkStatus);
    display.setCursor(64, 4),
        setColor(timeDelta >= storage.getNetworkThreshold()),
        display.print(timeDelta);

    setColor(false);
    display.setCursor(4, 4 + OLED_CHAR_HEIGHT),
        display.print(String(status.controlMsg.commandMsg.leftServoDegree) + "d " + String(status.controlMsg.commandMsg.leftMotorSpeed) + "%");
    display.setCursor(64, 4 + OLED_CHAR_HEIGHT),
        display.print(String(status.controlMsg.commandMsg.rightServoDegree) + "d " + String(status.controlMsg.commandMsg.rightMotorSpeed) + "%");

    if (status.boatMsg.type == BOAT_MSG::BOAT_INIT_MSG)
    {
        display.setCursor(4, 4 + 2 * OLED_CHAR_HEIGHT),
            display.print(status.boatMsg.initMsg.compileDatetime);
        display.setCursor(4, 4 + 3 * OLED_CHAR_HEIGHT),
            display.print(std::vector<String>({
                "SUCCESS",
                "I2C_FAILED",
                "SERVO0_FAILED",
                "SERVO1_FAILED",
                "MOTOR0_FAILED",
                "MOTOR1_FAILED",
                "BATTERY_FAILED",
                "MPU6050_FAILED",
            })[status.boatMsg.initMsg.status]);
    }
    else if (status.boatMsg.type == BOAT_MSG::BOAT_STATUS_MSG)
    {
        setColor(status.boatMsg.statusMsg.batteryPercentage <= storage.getBatteryThreshold());
        display.setCursor(4, 4 + 2 * OLED_CHAR_HEIGHT),
            display.print(String(status.boatMsg.statusMsg.batteryVoltage) + "V");
        display.setCursor(64, 4 + 2 * OLED_CHAR_HEIGHT),
            display.print(String(status.boatMsg.statusMsg.batteryPercentage) + "%");

        display.setCursor(4, 4 + 3 * OLED_CHAR_HEIGHT),
            display.print(status.boatMsg.statusMsg.mpuAX);
        display.setCursor(44, 4 + 3 * OLED_CHAR_HEIGHT),
            display.print(status.boatMsg.statusMsg.mpuAY);
        display.setCursor(84, 4 + 3 * OLED_CHAR_HEIGHT),
            display.print(status.boatMsg.statusMsg.mpuAZ);

        display.setCursor(4, 4 + 4 * OLED_CHAR_HEIGHT),
            setColor(abs(status.boatMsg.statusMsg.mpuGX) >= storage.getMpuGXThreshold()),
            display.print(status.boatMsg.statusMsg.mpuGX);
        display.setCursor(44, 4 + 4 * OLED_CHAR_HEIGHT),
            setColor(abs(status.boatMsg.statusMsg.mpuGY) >= storage.getMpuGYThreshold()),
            display.print(status.boatMsg.statusMsg.mpuGY);
        display.setCursor(84, 4 + 4 * OLED_CHAR_HEIGHT),
            setColor(abs(status.boatMsg.statusMsg.mpuGZ) <= storage.getMpuGZThreshold()),
            display.print(status.boatMsg.statusMsg.mpuGZ);
    }
}

/**
 * @brief OLED constructor
 * @param address I2C address of the OLED display
 * @param width Width of the display
 * @param height Height of the display
 */
OLED::OLED(uint8_t address, uint8_t width, uint8_t height, STORAGE &storage, A2D &a2d)
    : address(address), width(width), height(height),
      needUpdate(true), display(width, height, &Wire, -1), storage(storage)
{
    currentMenu = menu =
        new MENU("Main Menu",
                 {
                     new MENU("Boat basic", {
                                                new MENU("Limits", {
                                                                       new MENU("Max speed (%)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getMaxSpeed(); }, [&storage](MENU *menu)
                                                                                { storage.setMaxSpeed(menu->config.value); }, 0, 100),
                                                                       new MENU("Backward limit (%)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getBackwardLimit(); }, [&storage](MENU *menu)
                                                                                { storage.setBackwardLimit(menu->config.value); }, 0, 100),
                                                                       new MENU("Servo limit (d)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getServoLimit(); }, [&storage](MENU *menu)
                                                                                { storage.setServoLimit(menu->config.value); }, 30, 100),
                                                                   }),
                                                new MENU("Deltas", {
                                                                       new MENU("L servo delta (d)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getLeftServoDelta(); }, [&storage](MENU *menu)
                                                                                { storage.setLeftServoDelta(menu->config.value); }, -30, 30),
                                                                       new MENU("R servo delta (d)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getRightServoDelta(); }, [&storage](MENU *menu)
                                                                                { storage.setRightServoDelta(menu->config.value); }, -30, 30),
                                                                       new MENU("L motor delta (%)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getLeftMotorDelta(); }, [&storage](MENU *menu)
                                                                                { storage.setLeftMotorDelta(menu->config.value); }, -30, 30),
                                                                       new MENU("R motor delta (%)", [&storage](MENU *menu)
                                                                                { menu->config.value = storage.getRightMotorDelta(); }, [&storage](MENU *menu)
                                                                                { storage.setRightMotorDelta(menu->config.value); }, -30, 30),
                                                                   }),
                                                new MENU("Diff speed", {
                                                                           new MENU("Enable DS", [&storage](MENU *menu)
                                                                                    { menu->config.value = storage.getEnableDS(); }, [&storage](MENU *menu)
                                                                                    { storage.setEnableDS(menu->config.value); }, 0, 1),
                                                                           new MENU("DS rate (%)", [&storage](MENU *menu)
                                                                                    { menu->config.value = storage.getDSRate(); }, [&storage](MENU *menu)
                                                                                    { storage.setDSRate(menu->config.value); }, 0, 100),
                                                                       }),
                                            }),
                     new MENU("Boat advanced", {
                                                   new MENU("Changing these"),
                                                   new MENU("requires a reboot"),
                                                   new MENU("of the boat"),
                                                   new MENU(""),
                                                   new MENU("Timeout (s)", [&storage](MENU *menu)
                                                            { menu->config.value = storage.getControlTimeout(); }, [&storage](MENU *menu)
                                                            { storage.setControlTimeout(menu->config.value); }, 3, 10),
                                                   new MENU("L motor dir", [&storage](MENU *menu)
                                                            { menu->config.value = storage.getLeftMotorDir(); }, [&storage](MENU *menu)
                                                            { storage.setLeftMotorDir(menu->config.value); }, 0, 1),
                                                   new MENU("R motor dir", [&storage](MENU *menu)
                                                            { menu->config.value = storage.getRightMotorDir(); }, [&storage](MENU *menu)
                                                            { storage.setRightMotorDir(menu->config.value); }, 0, 1),
                                               }),
                     new MENU("Control", {
                                             new MENU("Dir threshold (%)", [&storage](MENU *menu)
                                                      { menu->config.value = storage.getDirThreshold(); }, [&storage](MENU *menu)
                                                      { storage.setDirThreshold(menu->config.value); }, 20, 90),
                                             new MENU("Calibrate joystick", [&a2d](MENU *menu)
                                                      { menu->updateDisplay(), a2d.calibrate(*menu->display); }),
                                             new MENU("Display", {
                                                                     new MENU("Network THLD (ms)", [&storage](MENU *menu)
                                                                              { menu->config.value = storage.getNetworkThreshold(); }, [&storage](MENU *menu)
                                                                              { storage.setNetworkThreshold(menu->config.value); }, 1000, 10000),
                                                                     new MENU("Battery THLD (%)", [&storage](MENU *menu)
                                                                              { menu->config.value = storage.getBatteryThreshold(); }, [&storage](MENU *menu)
                                                                              { storage.setBatteryThreshold(menu->config.value); }, 30, 70),
                                                                     new MENU("MPU X THLD", [&storage](MENU *menu)
                                                                              { menu->config.value = storage.getMpuGXThreshold(); }, [&storage](MENU *menu)
                                                                              { storage.setMpuXThreshold(menu->config.value); }, 1, 7),
                                                                     new MENU("MPU Y THLD", [&storage](MENU *menu)
                                                                              { menu->config.value = storage.getMpuGYThreshold(); }, [&storage](MENU *menu)
                                                                              { storage.setMpuYThreshold(menu->config.value); }, 1, 7),
                                                                     new MENU("MPU Z THLD", [&storage](MENU *menu)
                                                                              { menu->config.value = storage.getMpuGZThreshold(); }, [&storage](MENU *menu)
                                                                              { storage.setMpuZThreshold(menu->config.value); }, 7, 10),
                                                                 }),
                                         }),
                     new MENU("Reset to default", {
                                                      new MENU("Are you sure?"),
                                                      new MENU("Yes, reset", [&storage, this](MENU *menu)
                                                               {
                                                                   storage.reset();
                                                                   display.clearDisplay();
                                                                   setColor(false);
                                                                   display.setCursor(0, 0);
                                                                   display.println("Done");
                                                                   display.display();
                                                                   delay(500); }),
                                                  }),
                     new MENU("About", {
                                           new MENU("Innovation"),
                                           new MENU(""),
                                           new MENU("Repo URL", {
                                                                    new MENU("https://github.com"),
                                                                    new MENU("/langningchen"),
                                                                    new MENU("/innovation"),
                                                                }),
                                           new MENU("Authors", {
                                                                   new MENU("langningchen"),
                                                                   new MENU("lmyzzzz"),
                                                                   new MENU("zzsqjdhqgb"),
                                                                   new MENU("SenkoNekomimiBeat"),
                                                               }),
                                           new MENU("Open source license", {
                                                                               new MENU("GNU General Public"),
                                                                               new MENU("License Version 3"),
                                                                           }),
                                           new MENU("Compile time", {
                                                                        new MENU(__DATE__),
                                                                        new MENU(__TIME__),
                                                                    }),
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
OLED::PAGE OLED::getPage()
{
    return page;
}

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
void OLED::updateStatus(STATUS status)
{
    this->status = status, needUpdate = true;
}
