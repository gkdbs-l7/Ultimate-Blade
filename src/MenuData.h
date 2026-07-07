#pragma once
#include "UIState.h"

struct MenuItem
{
    const char *label;
    Screen targetScreen;
};

struct Menu
{
    const char *title;
    const MenuItem *items;
    int itemCount;
};

const MenuItem SETTINGS_ITEMS[] = {
    {"Back", SCREEN_MAIN},
    {"Bright", SCREEN_SETTING_BRIGHTNESS},
    {"Connect", SCREEN_CONNECTIVITY},
    {"Bat info", SCREEN_SETTING_BATTERY},
    {"Update", SCREEN_SETTING_UPDATE},
    {"About", SCREEN_ABOUT}};

const Menu SETTINGS_MENU = {
    "Settings",
    SETTINGS_ITEMS,
    6};

const MenuItem SETTING_CONNECTIVITY_ITEMS[] = {
    {"Back", SCREEN_SETTINGS},
    {"WIFI", SCREEN_SETTING_WIFI},
    {"ESP-NOW", SCREEN_SETTING_ESP_NOW}};

const Menu SETTING_CONNECTIVITY_MENU = {
    "Connectivity",
    SETTING_CONNECTIVITY_ITEMS,
    3};


const MenuItem SETTING_BRIGHTNESS_ITEMS[] = {
    {"Back", SCREEN_SETTINGS},
    {"Display", SCREEN_SETTING_BRIGHTNESS_OLED},
    {"Blade", SCREEN_SETTING_BRIGHTNESS_BLADE}};

const Menu SETTING_BRIGHTNESS_MENU = {
    "Brightness",
    SETTING_BRIGHTNESS_ITEMS,
    3};



    //나중에 realvalue로 수정
const MenuItem SETTING_WIFI_ITEMS[] = {
    {"Back", SCREEN_CONNECTIVITY},
    {"ADD WIFI", SCREEN_SETTING_WIFI_ADD}};

const Menu SETTING_WIFI_MENU = {
    "WIFI",
    SETTING_WIFI_ITEMS,
    2};