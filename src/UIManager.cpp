#include "UIManager.h"

/*void UIManager::begin() {
    return;
}*/

Screen UIManager::getScreen() const {
    return currentScreen;
}

Menu UIManager::getMenu() const {
    return currentMenu;
}

int UIManager::getSelectedMenuIndex() const {
    return menuSelectedIndex;
}

int UIManager::getScrollOffset() const {
    return menuScrollOffset;
}


void UIManager::onRotate(int delta) {

    switch (currentScreen) {
        case SCREEN_MAIN:

            //색변환로직
            break;
    
        case SCREEN_SETTINGS:
        case SCREEN_CONNECTIVITY:

            if (delta > 0 && menuSelectedIndex != currentMenu.itemCount - 1) {

                if (menuSelectedIndex == menuScrollOffset + 1) {
                    menuScrollOffset += 1;
                    menuSelectedIndex += 1;
                } else {
                    menuSelectedIndex += 1;
                }

            } else if (delta < 0 && menuSelectedIndex != 0) {

                if (menuSelectedIndex == menuScrollOffset) {
                    menuScrollOffset -= 1;
                    menuSelectedIndex -= 1;
                } else {
                    menuSelectedIndex -= 1;
                }

            }

            break;
    }
    return;
}

void UIManager::onClick() {

    switch (currentScreen) {
        case SCREEN_MAIN:

            currentScreen = SCREEN_SETTINGS;
            currentMenu = SETTINGS_MENU;
            menuSelectedIndex = 0;
            menuScrollOffset = 0;

            break;
    
        case SCREEN_SETTINGS:

            switch (currentMenu.items[menuSelectedIndex].targetScreen) {
                case SCREEN_MAIN:
                    currentScreen = SCREEN_MAIN;
                    break;

                case SCREEN_SETTING_BATTERY:
                    currentScreen = SCREEN_SETTING_BATTERY;
                    break;

                case SCREEN_ABOUT:
                    currentScreen = SCREEN_ABOUT;
                    break;

                case SCREEN_CONNECTIVITY:
                    currentScreen = SCREEN_CONNECTIVITY;
                    currentMenu = SETTINGS_CONNECTIVITY_MENU;
                    menuSelectedIndex = 0;
                    menuScrollOffset = 0;
                    
                    break;
            }
            break;
        case SCREEN_SETTING_BATTERY:
            
            currentScreen = SCREEN_SETTINGS;
            break;

        case SCREEN_ABOUT:
            
            currentScreen = SCREEN_SETTINGS;
            break;

        case SCREEN_CONNECTIVITY:

            switch (currentMenu.items[menuSelectedIndex].targetScreen) {
                case SCREEN_SETTINGS:
                    currentScreen = SCREEN_SETTINGS;
                    currentMenu = SETTINGS_MENU;
                    menuSelectedIndex = 0;
                    menuScrollOffset = 0;
                    break;

                case SCREEN_SETTING_WIFI:
                    break;

                case SCREEN_SETTING_ESP_NOW:
                    break;
                    
            }
            break;
    }

    return;
}

void UIManager::onLongPress() {

    //currentScreen = SCREEN_MAIN;

    return;
}