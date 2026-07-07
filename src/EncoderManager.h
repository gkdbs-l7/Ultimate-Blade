#pragma once

#include <Arduino.h>

class EncoderManager {
public:
    void begin();
    void update();

    int getDelta();

    bool isClicked();
    bool isLongPressed();

private:

    bool lastButtonState = true;
    unsigned long buttonDownTime = 0;

    bool clickFlag = false;
    bool longPressFlag = false;

};