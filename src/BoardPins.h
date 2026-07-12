#pragma once


#ifdef USE_ESP32_C3

#define OLED_SDA 8
#define OLED_SCL 9

#define BATTERY_PIN 0

#define ENC_CLK 1
#define ENC_DT  2
#define ENC_SW  3

#define LED_R 5
#define LED_G 6
#define LED_B 7

#define VIBRATION_PIN 10;

#else

#define LED_R 25
#define LED_G 32
#define LED_B 33

#define OLED_SDA 21
#define OLED_SCL 22

#define ENC_CLK 16
#define ENC_DT  17
#define ENC_SW  18

#define BATTERY_PIN 34

#endif