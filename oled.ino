#include <oled.hpp>
#include <define.hpp>

/**
 * @brief MENU constructor
 * @param name  Name of the menu
 */
OLED::MENU::MENU(String name, std::initializer_list<MENU *> subMenu, Adafruit_SSD1306 *display)
    : index(0), parent(nullptr), next(nullptr), prev(nullptr),
      name(name), display(display)
{
    for (MENU *item : subMenu)
        addSubMenu(item);
}

/**
 * @brief MENU destructor
 */
OLED::MENU::~MENU()
{
    for (MENU *item : subMenu)
        delete item;
    subMenu.clear();
}

/**
 * @brief Add a sub-menu to the menu
 * @param menu Sub-menu to add
 */
void OLED::MENU::addSubMenu(MENU *menu)
{
    if (subMenu.size() > 0)
    {
        subMenu.back()->next = menu;
        menu->prev = subMenu.back();
    }
    menu->parent = this, menu->display = display;
    subMenu.push_back(menu);
}

/**
 * @brief Render the configuration menu
 */
void OLED::MENU::render()
{
    display->setCursor(0, 0);
    display->setTextSize(1);
    uint8_t page = index / OLED_LINE_CNT;
    for (uint8_t i = OLED_LINE_CNT * page;
         i < OLED_LINE_CNT * (page + 1) && i < subMenu.size();
         i++)
    {
        MENU *item = subMenu[i];
        if (i == index)
            display->setTextColor(BLACK, WHITE);
        else
            display->setTextColor(WHITE, BLACK);
        if (item->name.length() > display->width() / OLED_CHAR_WIDTH)
        {
            display->println(item->name.substring(0, display->width() / OLED_CHAR_WIDTH - 3) + "...");
            // if (i == index)
            // {
            //     display->startscrolldiagleft(i * OLED_CHAR_HEIGHT, (i + 1) * OLED_CHAR_HEIGHT - 1);
            //     Serial.println(i * OLED_CHAR_HEIGHT);
            //     Serial.println((i + 1) * OLED_CHAR_HEIGHT - 1);
            // }
        }
        else
            display->println(item->name);
    }
}

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
    display.println("To be implemented");
}

/**
 * @brief OLED constructor
 * @param address I2C address of the OLED display
 * @param width Width of the display
 * @param height Height of the display
 */
OLED::OLED(uint8_t address, uint8_t width, uint8_t height)
    : address(address), width(width), height(height),
      needUpdate(true), display(width, height, &Wire, -1)
{
    currentMenu = menu =
        new MENU("Main Menu",
                 {
                     new MENU("Menu"),
                     new MENU("Very long long long Menu"),
                     new MENU("Sub Menu", {
                                              new MENU("Sub Menu 1"),
                                              new MENU("Sub Menu 2"),
                                              new MENU("Sub Menu 3"),
                                              new MENU("Sub Menu 4"),
                                              new MENU("Sub Menu 5"),
                                              new MENU("Sub Menu 6"),
                                              new MENU("Sub Menu 7"),
                                              new MENU("Sub Menu 8"),
                                              new MENU("Sub Menu 9"),
                                              new MENU("Sub Menu 10"),
                                          }),
                 },
                 &display);
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
    // display.stopscroll();
    switch (page)
    {
    case INIT:
        renderInit();
        break;
    case CONFIG:
        renderConfig();
        break;
    case STATUS:
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
 * @brief Move up in the configuration menu
 */
void OLED::configUp()
{
    if (currentMenu->index > 0)
    {
        currentMenu->index--;
        needUpdate = true;
    }
    else if (currentMenu->subMenu.size() > 0)
    {
        currentMenu->index = currentMenu->subMenu.size() - 1;
        needUpdate = true;
    }
}

/**
 * @brief Move down in the configuration menu
 */
void OLED::configDown()
{
    if (currentMenu->index < currentMenu->subMenu.size() - 1)
    {
        currentMenu->index++;
        needUpdate = true;
    }
    else if (currentMenu->subMenu.size() > 0)
    {
        currentMenu->index = 0;
        needUpdate = true;
    }
}

/**
 * @brief Go back in the configuration menu
 */
void OLED::configBack()
{
    if (currentMenu->parent)
    {
        currentMenu = currentMenu->parent;
        needUpdate = true;
    }
}

/**
 * @brief Enter the configuration menu
 */
void OLED::configEnter()
{
    if (currentMenu->subMenu.size() > 0)
    {
        currentMenu = currentMenu->subMenu[currentMenu->index];
        needUpdate = true;
    }
}
