#pragma once

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "UIManager.h"
#include "BatteryManager.h"
#include "LedManager.h"
#include "WiFiManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

class DisplayManager {
public:
    void begin();
    void rander(const UIManager& ui, const BatteryManager& battery, LedManager led, WiFiManager wifi);

private:
    Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    unsigned long lastUpdate = 0;
    const unsigned long updateInterval = 50;

    //void render(UIManager& ui);

    void drawMain(LedManager led);
    void drawMenu(const UIManager& ui);
    void drawOption(const UIManager& ui);
    void drawDisplayBrightControl(const UIManager& ui);
    void drawBladeBrightControl(LedManager led);
    void drawBatteryInfo(BatteryManager battery);
    //void drawWIFI(const UIManager& ui);
    void drawAbout();
    void drawConnectionState(WiFiManager wifi);
    int findSpaceForCenter(String text);

    void drawHeader(const String title);
    //void drawBatteryIcon(float percent);
    //void drawMenuList(MenuState menu);


    void applyBrightness(int brightness);
    int lastBrightness = 255;
};