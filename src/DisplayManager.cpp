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

            drawSettings(ui.getMenu());
            break;

        case SCREEN_SETTING_BATTERY:

            drawBatteryInfo(battery);
            break;        

    }
}

void DisplayManager::drawMain() {

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("MAIN~");
    display.display();

}

void DisplayManager::drawBatteryInfo(BatteryManager battery) {
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("Bat Info");

    display.setTextSize(1);
    display.setCursor(0, 20);
    display.println("Bat Capacity: 900mAh");
    display.print("Bat Voltage: ");
    display.print(battery.getVoltage(), 2);
    display.println("V");
    display.print("Bat Persentage: ");
    display.print(battery.getPercent());
    display.println("%");
    display.print("isCharging: ");
    display.println("No");

    display.display();

}

void DisplayManager::drawSettings(MenuState menu) {
    return;
}