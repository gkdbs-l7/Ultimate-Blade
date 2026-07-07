#pragma once

class BatteryManager {
public:
    void begin();
    void update();

    float getVoltage() const;
    int getPercent() const;

private:
    static const int SAMPLE_COUNT = 30;

    int samples[SAMPLE_COUNT];
    int sampleIndex = 0;
    long sampleSum = 0;

    float voltage = 0;
    float percent = 0;

    float batteryPercentFromVoltage(float v);

    unsigned long lastBatteryUpdateTime = 0;
};