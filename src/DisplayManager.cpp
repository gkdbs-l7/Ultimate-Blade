#include "DisplayManager.h"
#include "BatteryManager.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void DisplayManager::begin() {
    Wire.begin(21, 22); // ESP32 기본 SDA, SCL
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED init failed");
        while (true);
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

void DisplayManager::rander(const UIManager& ui, const BatteryManager& battery) {
    if (millis() - lastUpdate < updateInterval) {
        return;
    }

    lastUpdate = millis();

    
    display.clearDisplay();

    switch (ui.getScreen()) {
        case SCREEN_MAIN:

            drawMain();
            break;

        case SCREEN_SETTINGS:

            drawMenu(ui);
            break;

        case SCREEN_SETTING_BATTERY:

            drawBatteryInfo(battery);
            break; 
            
        case SCREEN_ABOUT:

            drawAbout();
            break;

        case SCREEN_CONNECTIVITY:

            drawMenu(ui);
            break;

    }

    display.display();
}

void DisplayManager::drawMain() {

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("MAIN~");

}

void DisplayManager::drawBatteryInfo(BatteryManager battery) {
    
    drawHeader("Battery Information");

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

void DisplayManager::drawMenu(const UIManager& ui) {

    Menu menu = ui.getMenu();

    int offset = ui.getScrollOffset();
    int selected = ui.getSelectedMenuIndex();

    drawHeader(menu.title);

    const int itemY = 18;
    const int itemH = 20;
    const int visibleCount = 2;

    for (int i = 0; i < visibleCount; i++) {
        int itemIndex = offset + i;
        if (itemIndex >= menu.itemCount) break;

        int y = itemY + i * itemH;
        bool isSelected = (itemIndex == selected);

        if (isSelected) {
            display.fillRoundRect(0, y - 2, 116, 18, 3, SSD1306_WHITE);
            display.setTextColor(SSD1306_BLACK);
        } else {
            display.setTextColor(SSD1306_WHITE);
        }

        display.setTextSize(2);
        display.setCursor(4, y);
        display.println(menu.items[itemIndex].label);

        display.setTextColor(SSD1306_WHITE);
    }

    // 스크롤바
    if (menu.itemCount > visibleCount) {
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

void DisplayManager::drawAbout() {

    display.setTextColor(SSD1306_WHITE);
    display.clearDisplay();

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

void DisplayManager::drawHeader(const String title) {
    display.setTextSize(1);
    display.setCursor(findSpaceForCenter(title), 0);
    display.println(title);
    display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
}

int DisplayManager::findSpaceForCenter(String text) {
  int16_t x1, y1;
  uint16_t width, height;

  // 글자가 차지하는 가로/세로 크기(pixel) 계산
  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  // 화면 가로 길이(예: 128)의 절반에서 글자 너비의 절반을 뺀 위치를 계산
  int center_x = (display.width() - width) / 2;

  return center_x;
}
