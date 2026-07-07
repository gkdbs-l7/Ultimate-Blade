#pragma once

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "UIManager.h"
#include "BatteryManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

class DisplayManager {
public:
    void begin();
    void rander(const UIManager& ui, const BatteryManager& battery);

private:
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    unsigned long lastUpdate = 0;
    const unsigned long updateInterval = 50;

    //void render(UIManager& ui);

    void drawMain();
    void drawMenu(const UIManager& ui);
    //void drawColor(UIManager& ui);
    void drawBatteryInfo(BatteryManager battery);
    //void drawBLE(UIManager& ui);
    //void drawAbout();

    //void drawHeader(const char* title);
    //void drawBatteryIcon(float percent);
    //void drawMenuList(MenuState menu);
};