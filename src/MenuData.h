#pragma once
#include "UIState.h"

struct MenuItem {
    const char* label;
    Screen targetScreen;
};

struct Menu {
    const char* title;
    const MenuItem* items;
    int itemCount;
};

const MenuItem SETTINGS_ITEMS[] = {
    {"Back", SCREEN_MAIN},
    {"Connect", SCREEN_CONNECTIVITY},
    {"Bat info", SCREEN_SETTING_BATTERY},
    {"Update", SCREEN_SETTING_UPDATE},
    {"About", SCREEN_ABOUT}
};

const Menu SETTINGS_MENU = {
    "Settings",
    SETTINGS_ITEMS,
    5
};