#pragma once

#include "WiFiManager.h"

#include <Arduino.h>

struct EspNowColorPacket {
    uint8_t type;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t brightness;
};

class EspNowManager {
public:
    enum SyncMode {
        SYNC_OFF,
        SYNC_SEND,
        SYNC_RECEIVE
    };

    void setMode(SyncMode mode, WiFiManager& wifi);
    SyncMode getMode() const;

    void sendColor(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);

    bool hasNewColor();
    EspNowColorPacket getLastColorPacket();

private:
    SyncMode mode = SYNC_OFF;

    void begin(WiFiManager& wifi);
    void end(WiFiManager& wifi);
    bool ensureInitialized(WiFiManager& wifi);

    bool initialized = false;

    static bool newColorReceived;
    static EspNowColorPacket lastPacket;

    static void onReceive(const uint8_t* mac, const uint8_t* data, int len);
};