#pragma once

#include "logos/Roselia.h"
#include "logos/Popipa.h"
#include "logos/Morfo.h"

#include <Arduino.h>

struct BladeColor
{
    int r;
    int g;
    int b;
    String name;
};

struct ColorGroup
{
    const BladeColor *bladeColors;
    const unsigned char *logo;
    int colorCount;
};

const BladeColor ROSELIA_BLADECOLOR[] = {
    {136, 17, 136, "Minato Youkina"},
    {0, 170, 187, "Hikawa Sayo"},
    {221, 34, 0, "Imai Risa"},
    {221, 0, 136, "Udagawa Ako"},
    {187, 187, 187, "Shirokane Rinko"},
    {51, 68, 170, "Roselia"}
};

const ColorGroup ROSELIA = {
    ROSELIA_BLADECOLOR,
    roseliaLogo,
    6
};

const BladeColor POPPINPARTY_BLADECOLOR[] = {
    {255, 85, 134, "Toyama Kasumi"},
    {0, 119, 221, "Hanazono Tae"},
    {255, 85, 187, "Ushigome Rimi"},
    {255, 204, 17, "Yamabuki Saya"},
    {170, 102, 221, "Ichigaya Arisa"},
    {255, 51, 119, "Poppin'Party"}
};

const ColorGroup POPPINPARTY = {
    POPPINPARTY_BLADECOLOR,
    popipaLogo,
    6
};

const BladeColor MORFONICA_BLADECOLOR[] = {
    {102, 119, 204, "Kurata Mashiro"},
    {238, 102, 102, "Kirigaya Touko"},
    {238, 119, 68, "Hiromachi Nanami"},
    {238, 119, 136, "Futaba Tsukushi"},
    {102, 153, 136, "Yashio Rui"},
    {51, 170, 255, "Morfonica"}
};

const ColorGroup MORFONICA = {
    MORFONICA_BLADECOLOR,
    morfoLogo,
    6
};