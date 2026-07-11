#include "LedManager.h"

#include <Arduino.h>
#include <BoardPins.h>

#define CH_R 0
#define CH_G 1
#define CH_B 2

void LedManager::begin() {
    ledcSetup(CH_R, 5000, 8);
    ledcSetup(CH_G, 5000, 8);
    ledcSetup(CH_B, 5000, 8);

    ledcAttachPin(LED_R, CH_R);
    ledcAttachPin(LED_G, CH_G);
    ledcAttachPin(LED_B, CH_B);
    return;
}

void LedManager::update() {
    if (current.r == target.r && current.g == target.g && current.b == target.b) {
        return;
    }

    unsigned long now = millis();
    unsigned long elapsed = now - fadeStartTime;

    if (elapsed >= fadeDuration) {
        current = target;

        applyColor();
        return;
    }

    float t = elapsed / (float)fadeDuration;

    current.r = fadeStartColor.r + (target.r - fadeStartColor.r) * t;
    current.g = fadeStartColor.g + (target.g - fadeStartColor.g) * t;
    current.b = fadeStartColor.b + (target.b - fadeStartColor.b) * t;

    applyColor();
}

void LedManager::setColorGroupFade(ColorGroup cg) {
    colorGroup = cg;
    colorIndex = 0;
    setTargetColor(colorGroup.bladeColors[colorIndex]);
    return;
}

const ColorGroup LedManager::getColorGroup() {
    return colorGroup;
}

void LedManager::nextColorFade() {
    if (colorIndex >= colorGroup.colorCount - 1) {
        colorIndex = 0;
    } else {
        colorIndex += 1;
    }
    setTargetColor(colorGroup.bladeColors[colorIndex]);
    return;
}

void LedManager::prevColorFade() {
    if (colorIndex == 0) {
        colorIndex = colorGroup.colorCount - 1;
    } else {
        colorIndex -= 1;
    }
    setTargetColor(colorGroup.bladeColors[colorIndex]);
    return;
}

int LedManager::getColorIndex() {
    return colorIndex;
}

void LedManager::setTargetColor(BladeColor bc) { //클래스 내부용임. 오류가능성있으니 유출금지
    //Serial.print("다음으로 색상 설정됨: ");
    //Serial.println(bc.name);
    target = bc;
    fadeStartTime = millis();
    fadeStartColor = current;

    stateChanged = true;
}

void LedManager::applyColor() {
    int r = current.r * brightness / 255;
    int g = current.g * brightness / 255;
    int b = current.b * brightness / 255;

    setLED(r, g, b);
}

void LedManager::setBrightness(int b) {
    brightness = constrain(b, 0, 255);
    applyColor();
    stateChanged = true;
    return;
}

int LedManager::getBrightness() {
    return brightness;
}

bool LedManager::hasStateChanged() {
    if (!stateChanged) {
        return false;
    }

    stateChanged = false;
    return true;
}

BladeColor LedManager::getTargetColor() const {
    return target;
}

void LedManager::setLED(int r, int g, int b) {
    ledcWrite(CH_R, constrain(r, 0, 255));
    ledcWrite(CH_G, constrain(g, 0, 255));
    ledcWrite(CH_B, constrain(b, 0, 255));
}