#include "UIManager.h"
#include "LedManager.h"

#include <Arduino.h>

/*void UIManager::begin() {
    return;
}*/

Screen UIManager::getScreen() const
{
    return currentScreen;
}

Menu UIManager::getMenu() const
{
    return currentMenu;
}

int UIManager::getSelectedMenuIndex() const
{
    return menuSelectedIndex;
}

int UIManager::getScrollOffset() const
{
    return menuScrollOffset;
}

int UIManager::getDisplayBrightness() const
{
    return displayBrightness;
}

void UIManager::changeDisplayBrightness(int delta) {
    displayBrightness = constrain(displayBrightness + delta, 0, 255);
}

void UIManager::onRotate(int delta)
{
    if (delta >= 2) delta = 1;
    if (delta <= -2) delta = -1;

    switch (currentScreen)
    {
    case SCREEN_MAIN:

        // 색변환로직 은 main.cpp에 있다와
        break;

    case SCREEN_SETTINGS:
    case SCREEN_CONNECTIVITY:
    case SCREEN_SETTING_WIFI:
    case SCREEN_SETTING_BRIGHTNESS:

        if (delta > 0 && menuSelectedIndex != currentMenu.itemCount - 1)
        {

            if (menuSelectedIndex == menuScrollOffset + 1)
            {
                menuScrollOffset += 1;
                menuSelectedIndex += 1;
            }
            else
            {
                menuSelectedIndex += 1;
            }
        }
        else if (delta < 0 && menuSelectedIndex != 0)
        {

            if (menuSelectedIndex == menuScrollOffset)
            {
                menuScrollOffset -= 1;
                menuSelectedIndex -= 1;
            }
            else
            {
                menuSelectedIndex -= 1;
            }
        }

        break;

        case SCREEN_SETTING_BRIGHTNESS_OLED:
            changeDisplayBrightness(delta * 5);
            break;

        case SCREEN_SETTING_BRIGHTNESS_BLADE:
            // main.cpp 에서 처리
            break;


    }
    return;
}

void UIManager::onClick()
{

    switch (currentScreen)
    {
    case SCREEN_MAIN:

        //currentScreen = SCREEN_SETTINGS;
        //currentMenu = SETTINGS_MENU;
        //menuSelectedIndex = 0;
        //menuScrollOffset = 0;

        break;

    case SCREEN_SETTINGS:

        switch (currentMenu.items[menuSelectedIndex].targetScreen)
        {
        case SCREEN_MAIN:
            currentScreen = SCREEN_MAIN;
            break;

        case SCREEN_SETTING_BRIGHTNESS:
            currentScreen = SCREEN_SETTING_BRIGHTNESS;
            currentMenu = SETTING_BRIGHTNESS_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;
            break;

        case SCREEN_SETTING_BATTERY:
            currentScreen = SCREEN_SETTING_BATTERY;
            break;

        case SCREEN_ABOUT:
            currentScreen = SCREEN_ABOUT;
            break;

        case SCREEN_CONNECTIVITY:
            currentScreen = SCREEN_CONNECTIVITY;
            currentMenu = SETTING_CONNECTIVITY_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;

            break;
        }
        break;
    case SCREEN_SETTING_BATTERY:
    case SCREEN_ABOUT:

        currentScreen = SCREEN_SETTINGS;
        break;

    case SCREEN_CONNECTIVITY:

        switch (currentMenu.items[menuSelectedIndex].targetScreen)
        {
        case SCREEN_SETTINGS:
            currentScreen = SCREEN_SETTINGS;
            currentMenu = SETTINGS_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;
            break;

        case SCREEN_SETTING_WIFI:

            

            currentScreen = SCREEN_SETTING_WIFI;
            currentMenu = SETTING_WIFI_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;
            break;

        case SCREEN_SETTING_ESP_NOW:
            break;
        }
        break;

    case SCREEN_SETTING_WIFI:

        switch (currentMenu.items[menuSelectedIndex].targetScreen)
        {
        case SCREEN_CONNECTIVITY:
            currentScreen = SCREEN_CONNECTIVITY;
            currentMenu = SETTING_CONNECTIVITY_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;
            break;

        case SCREEN_SETTING_WIFI_ADD:

            break;
        }
        break;

    case SCREEN_SETTING_BRIGHTNESS:

        switch (currentMenu.items[menuSelectedIndex].targetScreen)
        {
        case SCREEN_SETTINGS:
            currentScreen = SCREEN_SETTINGS;
            currentMenu = SETTINGS_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;
            break;

        case SCREEN_SETTING_BRIGHTNESS_OLED:
            currentScreen = SCREEN_SETTING_BRIGHTNESS_OLED;
            break;

        case SCREEN_SETTING_BRIGHTNESS_BLADE:
            currentScreen = SCREEN_SETTING_BRIGHTNESS_BLADE;
            break;
        }
        break;
    
    case SCREEN_SETTING_BRIGHTNESS_OLED:
    case SCREEN_SETTING_BRIGHTNESS_BLADE:

        currentScreen = SCREEN_SETTING_BRIGHTNESS;
        break;
    }




    return;
}

void UIManager::onLongPress()
{

    switch (currentScreen)
    {
    case SCREEN_MAIN:
        currentScreen = SCREEN_SETTINGS;
        currentMenu = SETTINGS_MENU;
        menuSelectedIndex = 0;
        menuScrollOffset = 0;
        break;
    }

    return;
}