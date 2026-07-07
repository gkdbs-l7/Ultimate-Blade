#include "BatteryManager.h"

#include <Arduino.h>

#define BATTERY_PIN 14

void BatteryManager::begin() {

    analogSetPinAttenuation(BATTERY_PIN, ADC_11db);

    sampleSum = 0;
    sampleIndex = 0;

    int value = analogReadMilliVolts(BATTERY_PIN);

    for(int i=0;i<SAMPLE_COUNT;i++){
        samples[i]=value;
        sampleSum+=value;
    }

}

void BatteryManager::update() {

    if (millis() - lastBatteryUpdateTime < 50) {
        return;
    }

    lastBatteryUpdateTime = millis();

    sampleSum -= samples[sampleIndex];

    samples[sampleIndex]=analogReadMilliVolts(BATTERY_PIN);

    sampleSum += samples[sampleIndex];

    sampleIndex++;

    if(sampleIndex>=SAMPLE_COUNT)
        sampleIndex=0;

    voltage=(sampleSum/(float)SAMPLE_COUNT)/1000.0f;

    voltage*=(3.84 / 3.90)*2.0f;

    percent=batteryPercentFromVoltage(voltage);

}

float BatteryManager::getVoltage() const {
    return voltage;
}

int BatteryManager::getPercent() const {
    return (int)percent;
}

float BatteryManager::batteryPercentFromVoltage(float v) {
  struct Point {
    float voltage;
    float percent;
  };

  const Point curve[] = {
    {4.20, 100},
    {4.10, 90},
    {4.00, 80},
    {3.92, 70},
    {3.85, 60},
    {3.79, 50},
    {3.75, 40},
    {3.70, 30},
    {3.60, 20},
    {3.45, 10},
    {3.30, 5},
    {3.20, 0}
  };

  if (v >= 4.20) return 100;
  if (v <= 3.20) return 0;

  for (int i = 0; i < 11; i++) {
    float vHigh = curve[i].voltage;
    float pHigh = curve[i].percent;
    float vLow  = curve[i + 1].voltage;
    float pLow  = curve[i + 1].percent;

    if (v <= vHigh && v >= vLow) {
      float ratio = (v - vLow) / (vHigh - vLow);
      return pLow + ratio * (pHigh - pLow);
    }
  }

  return 0;
}