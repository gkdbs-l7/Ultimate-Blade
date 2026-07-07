#include "UIManager.h"

void UIManager::begin() {
    return;
}

Screen UIManager::getScreen() const {
    return currentScreen;
}

MenuState UIManager::getMenu() const {
    return currentMenu;
}

void UIManager::onRotate(int delta) {
    return;
}

void UIManager::onClick() {

    currentScreen = SCREEN_SETTING_BATTERY;

    return;
}

void UIManager::onLongPress() {

    currentScreen = SCREEN_MAIN;

    return;
}