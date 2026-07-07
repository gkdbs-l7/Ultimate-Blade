#include "BatteryManager.h"
#include "DisplayManager.h"
#include "UIManager.h"
#include "EncoderManager.h"

#include <Arduino.h>



BatteryManager battery;
DisplayManager display;
UIManager ui;
EncoderManager encoder;


void setup() {
  Serial.begin(115200);
  display.begin();

  delay(2000); //처음 켤때 adc 크게 측정됨 방지

  encoder.begin();
  battery.begin();
}



void loop() {

  encoder.update();
  battery.update();

  int delta = encoder.getDelta();

  if (delta != 0) {
      ui.onRotate(delta);
  }

  if (encoder.isClicked()) {
    ui.onClick();
  }

  if (encoder.isLongPressed()) {
    ui.onLongPress();
  }

  display.rander(ui, battery);

}
