#pragma once

#include "ColorData.h"

class LedManager {
public:
    void begin();
    void update();

    //void setColor(int r, int g, int b);
    //void setColor(BladeColor);

    void setColorGroupFade(ColorGroup);
    const ColorGroup getColorGroup();

    void nextColorFade();
    void prevColorFade();
    int getColorIndex();

    void setBrightness(int b);
    int getBrightness();

    void setTargetColor(BladeColor bc);

    void setLED(int r, int g, int b);

    //void setBrightness(int brightness);

    //void fadeToColor(int r, int g, int b, int duration = 500);

    //BladeColor getCurrentColor() const;

    bool hasStateChanged();
    BladeColor getTargetColor() const;

private:
    BladeColor current = BladeColor{
        255,
        255,
        255,
        "None"
    };
    BladeColor target = BladeColor{
        0,
        0,
        0,
        "None"
    };;
    

    void applyColor();

    ColorGroup colorGroup;
    int colorIndex = 0;

    unsigned long fadeStartTime;
    BladeColor fadeStartColor = BladeColor{
        0,
        0,
        0,
        "None"
    };;
    int fadeDuration = 500; //500ms

    int brightness = 255;

    bool stateChanged = false;


    //bool fading;
};