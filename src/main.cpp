#include "BatteryManager.h"
#include "DisplayManager.h"
#include "UIManager.h"
#include "EncoderManager.h"
#include "LedManager.h"
#include "ColorData.h"

#include <Arduino.h>



BatteryManager battery;
DisplayManager display;
UIManager ui;
EncoderManager encoder;
LedManager led;


void setup() {
  Serial.begin(115200);
  display.begin();

  delay(2000); //처음 켤때 adc 크게 측정됨 방지

  encoder.begin();
  battery.begin();
  led.begin();
}



void loop() {

  encoder.update();
  battery.update();

  int delta = encoder.getDelta();

  if (delta != 0) {
      ui.onRotate(delta);

      if (ui.getScreen() == SCREEN_MAIN) {
        if (delta > 0) led.nextColorFade();
        if (delta < 0) led.prevColorFade();
      } else if (ui.getScreen() == SCREEN_SETTING_BRIGHTNESS_BLADE) {
        if (delta > 0) led.setBrightness(led.getBrightness() + 5);
        if (delta < 0) led.setBrightness(led.getBrightness() - 5);
      }
      
  }

  if (encoder.isClicked()) {
    ui.onClick();

    if (ui.getScreen() == SCREEN_MAIN) {
      if (led.getColorGroup().logo == ROSELIA.logo) {//꼼수부리가 -> 로고명으로 그룹비교
        led.setColorGroupFade(POPPINPARTY);
      } else if (led.getColorGroup().logo == POPPINPARTY.logo) {//꼼수부리가 -> 로고명으로 그룹비교
        led.setColorGroupFade(MORFONICA);
      } else if (led.getColorGroup().logo == MORFONICA.logo) {//꼼수부리가 -> 로고명으로 그룹비교
        led.setColorGroupFade(ROSELIA);
      }
    }
  }

  if (encoder.isLongPressed()) {
    ui.onLongPress();
  }

  led.update();
  display.rander(ui, battery, led);

}
