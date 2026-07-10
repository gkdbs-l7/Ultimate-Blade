#include "EspNowManager.h"
#include "WiFiManager.h"

#include <WiFi.h>
#include <esp_now.h>


#define PACKET_TYPE_COLOR 1

static uint8_t broadcastAddress[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

bool EspNowManager::newColorReceived = false;
EspNowColorPacket EspNowManager::lastPacket = {};

void EspNowManager::begin(WiFiManager& wifi) {

    wifi.setMode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW init failed");
        mode = SYNC_OFF;
        wifi.setMode(WIFI_OFF);
        return;
    }

}

void EspNowManager::end(WiFiManager& wifi) {
    esp_now_deinit();
    initialized = false;
    wifi.setMode(WIFI_OFF);
    mode = SYNC_OFF;
}

bool EspNowManager::ensureInitialized(WiFiManager& wifi) {
    wifi.setMode(WIFI_STA);

    if (initialized) {
        return true;
    }

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW init failed");
        return false;
    }

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (!esp_now_is_peer_exist(broadcastAddress)) {
        if (esp_now_add_peer(&peerInfo) != ESP_OK) {
            Serial.println("Broadcast peer add failed");
            esp_now_deinit();
            return false;
        }
    }

    initialized = true;
    return true;
}

void EspNowManager::setMode(SyncMode newMode, WiFiManager& wifi) {

    if (newMode == mode) return;
    mode = newMode;

    if (newMode == SYNC_OFF) {
        end(wifi);
        return;
    }

    if (!ensureInitialized(wifi)) {
        mode = SYNC_OFF;
        wifi.setMode(WIFI_OFF);
        return;
    }

    if (newMode == SYNC_RECEIVE) {
        esp_now_register_recv_cb(onReceive);
        Serial.println("esp receive");
    }

    if (newMode == SYNC_SEND) {
        Serial.println("esp send");
    }

    
}

EspNowManager::SyncMode EspNowManager::getMode() const {
    return mode;
}

void EspNowManager::sendColor(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    if (mode != SYNC_SEND) return;

    EspNowColorPacket packet;
    packet.type = PACKET_TYPE_COLOR;
    packet.r = r;
    packet.g = g;
    packet.b = b;
    packet.brightness = brightness;

    esp_now_send(
        broadcastAddress,
        (uint8_t*)&packet,
        sizeof(packet)
    );
}

bool EspNowManager::hasNewColor() {
    if (mode != SYNC_RECEIVE) return false;

    if (newColorReceived) {
        newColorReceived = false;
        return true;
    }

    return false;
}

EspNowColorPacket EspNowManager::getLastColorPacket() {
    return lastPacket;
}

void EspNowManager::onReceive(const uint8_t* mac, const uint8_t* data, int len) {
    //Serial.println("뭔가옴");
    if (len != sizeof(EspNowColorPacket)) return;

    EspNowColorPacket packet;
    memcpy(&packet, data, sizeof(packet));

    if (packet.type != PACKET_TYPE_COLOR) return;

    lastPacket = packet;
    newColorReceived = true;
}