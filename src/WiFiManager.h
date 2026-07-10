#pragma once

#include <WiFi.h>

class WiFiManager {
public:

    /*enum WiFiMode {
        WIFI_OFF,
        WIFI_STA,
        WIFI_AP
    };*/ //이미 wifi.h에 있음


    //void begin();
    //void update();

    wifi_mode_t getMode() const;
    void setMode(wifi_mode_t mode);

    //bool isClicked();
    //bool isLongPressed();

private:

    wifi_mode_t currentMode = WIFI_OFF;

};