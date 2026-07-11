#include "DisplayManager.h"
#include "BatteryManager.h"
#include "LedManager.h"
#include "BoardPins.h"
#include "WiFiManager.h"
#include "EspNowManager.h"

#include "logos/Roselia.h"
#include "logos/Popipa.h"

#include "icons.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void DisplayManager::begin()
{
    Wire.begin(OLED_SDA, OLED_SCL); // ESP32 기본 SDA, SCL

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED init failed");
        while (true)
            ;
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Ultimate");
    display.setCursor(60, 20);
    display.println("Blade");
    display.setTextSize(1);
    display.println("Initing Processing...");
    display.println("Please Wait");
    display.println("software from 26.07");
    display.display();
}

void DisplayManager::rander(const UIManager &ui, const BatteryManager &battery, LedManager& led, WiFiManager& wifi, EspNowManager& espNow)
{
    if (millis() - lastUpdate < updateInterval)
    {
        return;
    }

    lastUpdate = millis();

    if (ui.getDisplayBrightness() != lastBrightness)
    {
        lastBrightness = ui.getDisplayBrightness();
        applyBrightness(lastBrightness);
    }

    display.clearDisplay();

    switch (ui.getScreen())
    {
    case SCREEN_MAIN:

        drawMain(led);
        break;

    case SCREEN_SETTINGS:
    case SCREEN_CONNECTIVITY:
    case SCREEN_SETTING_WIFI:
    case SCREEN_SETTING_BRIGHTNESS:

        drawMenu(ui);
        break;

    case SCREEN_SETTING_BATTERY:

        drawBatteryInfo(battery);
        break;

    case SCREEN_ABOUT:

        drawAbout();
        break;

    case SCREEN_SETTING_BRIGHTNESS_OLED:

        drawDisplayBrightControl(ui);
        break;

    case SCREEN_SETTING_BRIGHTNESS_BLADE:

        drawBladeBrightControl(led);
        break;

    case SCREEN_SETTING_ESP_NOW:
        
        drawOption(ui);
        break;
    }

    if (ui.getScreen() != SCREEN_MAIN) {
        drawConnectionIcon(wifi, espNow);
        drawBatteryIcon(battery);
    }

    display.display();
}

void DisplayManager::drawMain(LedManager led)
{
    ColorGroup cg = led.getColorGroup();
    int colorIndex = led.getColorIndex();

    display.setTextSize(1);
    display.setCursor(findSpaceForCenter(cg.bladeColors[colorIndex].name), 7);
    display.print(cg.bladeColors[colorIndex].name);

    display.drawBitmap(45, 17, cg.logo, 40, 40, SSD1306_WHITE);

    display.display();
}

void DisplayManager::drawBatteryInfo(BatteryManager battery)
{

    drawHeader("Battery Info");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 18);
    display.println("Bat Capacity: 900mAh");

    display.setCursor(0, 28);
    display.print("Bat Voltage: ");
    display.print(battery.getVoltage(), 2);
    display.println("V");

    display.setCursor(0, 38);
    display.print("Bat Persentage: ");
    display.print(battery.getPercent());
    display.println("%");

    display.setCursor(0, 48);
    display.print("isCharging: ");
    display.println("No");
}

void DisplayManager::drawMenu(const UIManager &ui)
{

    Menu menu = ui.getMenu();


    int offset = ui.getScrollOffset();
    int selected = ui.getSelectedMenuIndex();

    drawHeader(menu.title);

    const int itemY = 18;
    const int itemH = 20;
    const int visibleCount = 2;

    for (int i = 0; i < visibleCount; i++)
    {
        int itemIndex = offset + i;
        if (itemIndex >= menu.itemCount)
            break;

        int y = itemY + i * itemH;
        bool isSelected = (itemIndex == selected);

        if (isSelected)
        {
            display.fillRoundRect(0, y - 2, 116, 18, 3, SSD1306_WHITE);
            display.setTextColor(SSD1306_BLACK);
        }
        else
        {
            display.setTextColor(SSD1306_WHITE);
        }

        display.setTextSize(2);
        display.setCursor(4, y);
        display.println(menu.items[itemIndex].label);

        display.setTextColor(SSD1306_WHITE);
    }

    // 스크롤바
    if (menu.itemCount > visibleCount)
    {
        int barX = 124;
        int barY = 14;
        int barH = 48;
        const int barWidth = 4;

        display.drawRect(barX, barY, barWidth, barH, SSD1306_WHITE);

        int thumbH = max(8, barH * visibleCount / menu.itemCount);
        int thumbY = barY + (barH - thumbH) * selected / (menu.itemCount - 1);

        display.fillRect(barX + 1, thumbY, barWidth - 2, thumbH, SSD1306_WHITE);
    }

    return;
}

void DisplayManager::drawOption(const UIManager &ui)
{

    Option option = ui.getOption();

    int offset = ui.getScrollOffset();
    int selected = ui.getSelectedMenuIndex();

    drawHeader(option.title);

    display.setTextSize(1);
    display.setCursor(findSpaceForCenter(String(offset + 1) + " / " + String(option.itemCount)), 48);
    display.print(String(offset + 1) + " / " + String(option.itemCount));

    display.setTextSize(1);
    display.setCursor(findSpaceForCenter("Push to select"), 57);
    display.print("Push to select");

    display.fillTriangle(4, 32, 10, 26, 10, 38, SSD1306_WHITE);
    display.fillTriangle(123, 32, 117, 26, 117, 38, SSD1306_WHITE);

    if (selected == offset)
    {
        display.fillRoundRect(16, 21, 96, 22, 5, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
    }
    else
    {
        display.setTextColor(SSD1306_WHITE);
    }

    display.setTextSize(2);
    display.setCursor(findSpaceForCenter(option.items[offset].label), 25);
    display.print(option.items[offset].label);
}

void DisplayManager::drawAbout()
{

    display.setTextColor(SSD1306_WHITE);

    // 제목
    drawHeader("Ultimate Blade");

    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

    // Firmware
    display.setCursor(0, 18);
    display.print("FW : ");
    display.println("v0.1.0");

    // 제작자
    display.setCursor(0, 32);
    display.print("By : ");
    display.println("HaYoon");

    // Github
    display.setCursor(0, 46);
    display.println("github.com");
    display.setCursor(0, 56);
    display.println("gkdbs-l7");
}

void DisplayManager::drawDisplayBrightControl(const UIManager &ui)
{

    int brightness = ui.getDisplayBrightness();
    drawHeader("Display Bright");

    display.setTextSize(2);
    display.setCursor(findSpaceForCenter((String)brightness), 20);
    display.print(brightness);

    int barX = 8;
    int barY = 45;
    int barW = 112;
    int barH = 10;

    display.drawRoundRect(barX, barY, barW, barH, 2, SSD1306_WHITE);

    int fillW = map(brightness, 0, 255, 0, barW - 2);
    display.fillRect(barX + 1, barY + 1, fillW, barH - 2, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(findSpaceForCenter("Turn adjust"), 57);
    display.print("Turn adjust");
}

void DisplayManager::drawBladeBrightControl(LedManager led)
{

    int brightness = led.getBrightness();
    drawHeader("Blade Bright");

    display.setTextSize(2);
    display.setCursor(findSpaceForCenter((String)brightness), 20);
    display.print(brightness);

    int barX = 8;
    int barY = 45;
    int barW = 112;
    int barH = 10;

    display.drawRoundRect(barX, barY, barW, barH, 2, SSD1306_WHITE);

    int fillW = map(brightness, 0, 255, 0, barW - 2);
    display.fillRect(barX + 1, barY + 1, fillW, barH - 2, SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(findSpaceForCenter("Turn adjust"), 57);
    display.print("Turn adjust");
}

void DisplayManager::drawHeader(const String title)
{
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(findSpaceForCenter(title), 0);
    display.println(title);
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

}

void DisplayManager::drawConnectionIcon(WiFiManager& wifi, EspNowManager& espNow)
{
    //int dx = 120;
    int dx = 0;

    if (wifi.getMode() == WIFI_STA)
    {
        display.drawBitmap(dx, 0, wifiIcon, 8, 8, SSD1306_WHITE);
            dx += 8;

        if (espNow.getMode() == EspNowManager::SYNC_RECEIVE) {
            display.drawBitmap(dx, 0, espReceiveIcon, 8, 8, SSD1306_WHITE);
        } else if (espNow.getMode() == EspNowManager::SYNC_SEND) {
            display.drawBitmap(dx, 0, espSendIcon, 8, 8, SSD1306_WHITE);
        }
        //display.drawLine(dx, 0, dx + 7, 7, SSD1306_WHITE);
        //display.drawLine(dx + 7, 0, dx, 7, SSD1306_WHITE);
    }
    else if (wifi.getMode() == WIFI_AP)
    {
        display.setTextSize(1);
        display.setCursor(110, 0);
        display.setTextColor(SSD1306_WHITE);
        display.println("A");
        //display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
    }
}

void DisplayManager::drawBatteryIcon(BatteryManager battery) {
    const int persent = battery.getPercent();

    const int batX = 116;
    if (persent >= 80) {
        display.drawBitmap(batX, 0, batteryIcon100, 12, 8, SSD1306_WHITE);
    } else if (persent >= 60) {
        display.drawBitmap(batX, 0, batteryIcon80, 12, 8, SSD1306_WHITE);
    } else if (persent >= 40) {
        display.drawBitmap(batX, 0, batteryIcon60, 12, 8, SSD1306_WHITE);
    } else if (persent >= 20) {
        display.drawBitmap(batX, 0, batteryIcon40, 12, 8, SSD1306_WHITE);
    } else if (persent >= 10) {
        display.drawBitmap(batX, 0, batteryIcon20, 12, 8, SSD1306_WHITE);
    } else {
        display.drawBitmap(batX, 0, batteryIcon10, 12, 8, SSD1306_WHITE);
    }
    
}

int DisplayManager::findSpaceForCenter(String text)
{
    int16_t x1, y1;
    uint16_t width, height;

    // 글자가 차지하는 가로/세로 크기(pixel) 계산
    display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

    // 화면 가로 길이(예: 128)의 절반에서 글자 너비의 절반을 뺀 위치를 계산
    int center_x = (display.width() - width) / 2;

    return center_x;
}

void DisplayManager::applyBrightness(int brightness)
{
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(brightness);
}