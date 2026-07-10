#include "BatteryManager.h"
#include "DisplayManager.h"
#include "UIManager.h"
#include "EncoderManager.h"
#include "LedManager.h"
#include "ColorData.h"
#include "WiFiManager.h"
#include "EspNowManager.h"

#include <Arduino.h>

/*
  해야할것!!!!
  wifi sta -> ap 변경되는 경우가 있을때
  서로 협조잘하기(싫핼중인거 잘 종료)
*/

BatteryManager battery;
DisplayManager display;
UIManager ui;
EncoderManager encoder;
LedManager led;
WiFiManager wifi;
EspNowManager espNow;

void setup()
{
  Serial.begin(115200);
  display.begin();

  delay(2000); // 처음 켤때 adc 크게 측정됨 방지

  encoder.begin();
  battery.begin();
  led.begin();

  //espNow.setMode(EspNowManager::SYNC_RECEIVE, wifi);

}

void loop()
{

  encoder.update();
  battery.update();

  int delta = encoder.getDelta();

  if (delta != 0)
  {
    ui.onRotate(delta);

    if (ui.getScreen() == SCREEN_MAIN)
    {
      if (delta > 0)
        led.nextColorFade();
      if (delta < 0)
        led.prevColorFade();
    }
    else if (ui.getScreen() == SCREEN_SETTING_BRIGHTNESS_BLADE)
    {
      if (delta > 0)
        led.setBrightness(led.getBrightness() + 5);
      if (delta < 0)
        led.setBrightness(led.getBrightness() - 5);
    }
  }

  if (encoder.isClicked())
  {
    if (ui.getScreen() == SCREEN_MAIN)
    {
      if (led.getColorGroup().logo == ROSELIA.logo)
      { // 꼼수부리가 -> 로고명으로 그룹비교
        led.setColorGroupFade(POPPINPARTY);
      }
      else if (led.getColorGroup().logo == POPPINPARTY.logo)
      { // 꼼수부리가 -> 로고명으로 그룹비교
        led.setColorGroupFade(MORFONICA);
      }
      else if (led.getColorGroup().logo == MORFONICA.logo)
      { // 꼼수부리가 -> 로고명으로 그룹비교
        led.setColorGroupFade(ROSELIA);
      }
    }
    else if (ui.getScreen() == SCREEN_SETTING_ESP_NOW)
    {
      OptionAction action = ui.getOption().items[ui.getScrollOffset()].action;
      if (action == COLORSYNC_SYNC_OFF)
      {
        espNow.setMode(EspNowManager::SYNC_OFF, wifi);
      }
      else if (action == COLORSYNC_SYNC_SEND)
      {
        espNow.setMode(EspNowManager::SYNC_SEND, wifi);
      }
      else if (action == COLORSYNC_SYNC_RECEIVE)
      {
        espNow.setMode(EspNowManager::SYNC_RECEIVE, wifi);
      }
      ui.onClick();
    }
    else
    {
      ui.onClick();
    }
  }

  if (encoder.isLongPressed())
  {
    ui.onLongPress();
  }

  if (espNow.getMode() == EspNowManager::SYNC_RECEIVE && espNow.hasNewColor())
  {
    EspNowColorPacket packet = espNow.getLastColorPacket();
    led.setBrightness(packet.brightness);
    BladeColor bc = {
        packet.r,
        packet.g,
        packet.b,
        "broadcast"};
    led.setTargetColor(bc);
  }

  if (espNow.getMode() == EspNowManager::SYNC_SEND && led.hasStateChanged())
  {
    const BladeColor &color = led.getTargetColor();
    /*Serial.print("이거보냄 ");
    Serial.print(color.r);
    Serial.print(", ");
    Serial.print(color.g);
    Serial.print(", ");
    Serial.println(color.b);*/

    espNow.sendColor(
        color.r,
        color.g,
        color.b,
        led.getBrightness());
  }

  led.update();
  display.rander(ui, battery, led, wifi);
}
