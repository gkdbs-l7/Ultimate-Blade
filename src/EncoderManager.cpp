#include "EncoderManager.h"
#include <Arduino.h>

#define ENC_CLK 16
#define ENC_DT  17
#define ENC_SW  18

static volatile int encoderDelta = 0;
static volatile int lastCLK = HIGH;

void IRAM_ATTR encoderISR() {
    int clk = digitalRead(ENC_CLK);
    int dt  = digitalRead(ENC_DT);

    if (clk != lastCLK) {
        if (clk == LOW) {
            if (dt == HIGH) encoderDelta++;
            else encoderDelta--;
        }
        lastCLK = clk;
    }
}

void EncoderManager::begin() {
    pinMode(ENC_CLK, INPUT_PULLUP);
    pinMode(ENC_DT, INPUT_PULLUP);
    pinMode(ENC_SW, INPUT_PULLUP);

    lastCLK = digitalRead(ENC_CLK);

    attachInterrupt(
        digitalPinToInterrupt(ENC_CLK),
        encoderISR,
        CHANGE
    );
}

int EncoderManager::getDelta() {
    noInterrupts();
    int delta = encoderDelta;
    encoderDelta = 0;
    interrupts();

    return delta;
}

void EncoderManager::update() {
    bool current = digitalRead(ENC_SW);

    if (lastButtonState == HIGH && current == LOW) {
        buttonDownTime = millis();
    }

    if (lastButtonState == LOW && current == HIGH) {
        unsigned long pressTime = millis() - buttonDownTime;

        if (pressTime < 600) {
            clickFlag = true;
        } else {
            longPressFlag = true;
        }
    }

    lastButtonState = current;
}

bool EncoderManager::isClicked() {
    if (clickFlag) {
        clickFlag = false;
        return true;
    }
    return false;
}

bool EncoderManager::isLongPressed() {
    if (longPressFlag) {
        longPressFlag = false;
        return true;
    }
    return false;
}