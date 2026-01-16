#include <SoftwareSerial.h>

SoftwareSerial HC06(10, 11); // RX, TX
const int ENA_PIN = 6;
const int IN1_PIN = 4;
const int IN2_PIN = 5;
const float INITIAL_VOLTAGE = 0.8; // 초기 전압
const float TARGET_VOLTAGE = 0.4; // 목표 전압
const float SUPPLY_VOLTAGE = 3.0; // 공급 전압
const int PWM_MAX = 255; // 최대 PWM 값

void setup() {
  Serial.begin(9600);
  HC06.begin(9600);

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);

  int initialPWMValue = (int)(INITIAL_VOLTAGE / SUPPLY_VOLTAGE * PWM_MAX);
  analogWrite(ENA_PIN, initialPWMValue); // 모터 초기 1.5V로 구동
}

void loop() {
  if (HC06.available()) {
    String message = HC06.readStringUntil('\n');
    message.trim();

    if (message == "5cm 이하") {
      int pwmValue = (int)(TARGET_VOLTAGE / SUPPLY_VOLTAGE * PWM_MAX);
      analogWrite(ENA_PIN, pwmValue);
      Serial.println("모터 0.5V로 구동");
    }
  }
}
