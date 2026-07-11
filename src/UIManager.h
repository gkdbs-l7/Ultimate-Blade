#pragma once

#include "UIState.h"
#include "MenuData.h"

class UIManager {
public:
    //void begin();
    //void update();

    void onRotate(int delta);
    void onClick();
    void onLongPress();

    int getSelectedMenuIndex() const;
    int getScrollOffset() const;
    void setSelectedMenuIndex(int index);
    void setScrollOffset(int offset);
    Screen getScreen() const;
    Menu getMenu() const; //메뉴있는경우 가지고있는 정보 반환.
    Option getOption() const;

    int getDisplayBrightness() const;
    void changeDisplayBrightness(int delta);

    int colorDeltaValue;

    //void setBattery(float voltage, float percent);
    //float getBatteryVoltage() const;
    //float getBatteryPercent() const;

    /*void setHue(int hue);
    int getHue() const;

    void setBrightness(int brightness);
    int getBrightness() const;*/ //나중에 따로 뺄거임

private:
    Screen currentScreen = SCREEN_MAIN;
    Menu currentMenu;
    Option currentOption;
    

    //float batteryVoltage = 0.0f;
    //float batteryPercent = 0.0f;

    //int hue = 0;
    //int brightness = 128;

    int menuSelectedIndex = 0;
    int menuScrollOffset = 0;

    int displayBrightness = 255;

};