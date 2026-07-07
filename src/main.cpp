#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define BATTERY_PIN 14
#define ENC_CLK 16
#define ENC_DT  17
#define ENC_SW  18

volatile int encoderDelta = 0;
volatile int lastCLK = HIGH;

bool lastButtonState = HIGH;
bool buttonPressed = false;
unsigned long buttonDownTime = 0;

void IRAM_ATTR encoderISR() {
  int clk = digitalRead(ENC_CLK);
  int dt  = digitalRead(ENC_DT);

  if (clk != lastCLK) {
    if (clk == LOW) {
      if (dt == HIGH) encoderDelta++;
      else encoderDelta--;
    }
    lastCLK = clk;
  }
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum Screen {
  SCREEN_MAIN,
  SCREEN_SETTINGS,
  SCREEN_SETTING_BATTERY
};

Screen currentScreen = SCREEN_SETTING_BATTERY;

unsigned long lastBatteryUpdateTime = 0;
const int SAMPLE_COUNT = 30;
int samples[SAMPLE_COUNT];
int sampleIndex = 0;
long batteryVoltageSum = 0;

void initBatteryFilter() {

    float value = analogReadMilliVolts(BATTERY_PIN);

    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 40);
    display.println(value/1000.0*2, 2.0);
    display.display();

    for (int i = 0; i < SAMPLE_COUNT; i++) {
        samples[i] = value;
        batteryVoltageSum += value;
    }

    
}

void updateBatteryFilter() {

    batteryVoltageSum -= samples[sampleIndex];

    samples[sampleIndex] = analogReadMilliVolts(BATTERY_PIN);

    batteryVoltageSum += samples[sampleIndex];

    sampleIndex++;

    if(sampleIndex >= SAMPLE_COUNT)
        sampleIndex = 0;
}

float batteryPercentFromVoltage(float v) {
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

//chatgpt encoder func 시작
void setupEncoder() {
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  lastCLK = digitalRead(ENC_CLK);

  attachInterrupt(
    digitalPinToInterrupt(ENC_CLK),
    encoderISR,
    CHANGE
  );
}

int getEncoderDelta() {
  noInterrupts();
  int delta = encoderDelta;
  encoderDelta = 0;
  interrupts();

  return delta;
}

void updateEncoderButton() {
  bool current = digitalRead(ENC_SW);

  if (lastButtonState == HIGH && current == LOW) {
    buttonDownTime = millis();
    buttonPressed = true;
  }

  if (lastButtonState == LOW && current == HIGH) {
    unsigned long pressTime = millis() - buttonDownTime;

    if (pressTime < 600) {
      Serial.println("Click");
      // 선택
    } else {
      Serial.println("Long Press");
      // 뒤로가기
    }

    buttonPressed = false;
  }

  lastButtonState = current;
}
//chatgpt encoder func 끝

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // ESP32 기본 SDA, SCL

  analogSetPinAttenuation(BATTERY_PIN, ADC_11db);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Ultimate");
  display.setCursor(60, 20);
  display.println("Blade");
  display.setTextSize(1);
  display.println("Initing Processing...");
  display.println("Please Wait");
  display.println("software from 26.07");
  display.display();


  delay(2000); //처음 켤때 adc 크게 측정됨 방지

  setupEncoder();
  initBatteryFilter();
}

void randerScreen() {
  display.clearDisplay();

  switch (currentScreen) {
    case SCREEN_MAIN:
      break;

    case SCREEN_SETTING_BATTERY:
      float mv = analogReadMilliVolts(BATTERY_PIN) / 1000.0 * 2.0;

      float batteryVoltage = (batteryVoltageSum / SAMPLE_COUNT / 1000.0) * 2.0;

      float correction = 3.84 / 3.90;  // ≈ 0.9846
      batteryVoltage *= correction; //일단보정
      mv += correction;

      int batteryPercent = batteryPercentFromVoltage(batteryVoltage);

      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("Bat Info");

      display.setTextSize(1);
      display.setCursor(0, 20);
      display.println("Bat Capacity: 900mAh");
      display.print("Bat Voltage: ");
      display.print(batteryVoltage, 2);
      display.println("V");
      display.print("Bat Persentage: ");
      display.print(batteryPercent);
      display.println("%");
      display.print("isCharging: ");
      display.println("No");

      display.display();


      break;

  }


}

void handleEncoder


void loop() {

  if (millis() - lastBatteryUpdateTime >= 50) {

    lastBatteryUpdateTime = millis();

    updateBatteryFilter();

  }

  int delta = getEncoderDelta();

  if (delta > 0) {
    Serial.println("Right");
    // 메뉴 아래 / 값 증가
  } else if (delta < 0) {
    Serial.println("Left");
    // 메뉴 위 / 값 감소
  }

  updateEncoderButton();

  randerScreen();

}
