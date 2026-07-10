#include "WiFiManager.h"


void WiFiManager::setMode(wifi_mode_t mode) {

    if (mode == currentMode) {
        return;
    }
    
    currentMode = mode;

    switch (mode) {

        case WIFI_OFF:
            WiFi.disconnect(true, true);
            WiFi.mode(WIFI_OFF);
            Serial.println("wifi off");
            break;

        case WIFI_STA:
            WiFi.mode(WIFI_STA);
            Serial.println("wifi sta");
            break;

        case WIFI_AP:
            WiFi.mode(WIFI_AP);
            Serial.println("wifi ap");
            break;
    }
}

wifi_mode_t WiFiManager::getMode() const {
    return currentMode;
}