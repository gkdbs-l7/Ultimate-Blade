#pragma once

enum Screen {
    SCREEN_MAIN,
    SCREEN_SETTINGS,
    SCREEN_SETTING_BATTERY
};

struct MenuState {
    const char* title;
    const char** items;
    int itemCount;
    int selectedIndex;
    int scrollOffset;
};