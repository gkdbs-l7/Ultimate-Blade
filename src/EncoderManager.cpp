#include "EncoderManager.h"
#include <Arduino.h>
#include <BoardPins.h>

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

    if (lastButtonState == HIGH && current == LOW) { //눌렀을때
        buttonDownTime = millis();
    }  

    if (lastButtonState == LOW && current == LOW) {
        unsigned long pressTime = millis() - buttonDownTime;
        if (pressTime >= 600 && lastButtonState == false) {
            longPressFlag = true;
            lastButtonState = true;
        }
    }


    if (lastButtonState == LOW && current == HIGH) { //땠을때
        unsigned long pressTime = millis() - buttonDownTime;

        if (pressTime < 600) {
            clickFlag = true;
        } else {
            lastLongPressFlag = false;
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