#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "secret.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

// Motor and sensor pins
const int motorA = 14;
const int motorB = 12;
const int rainSensor_pin = A0;
const int rainPower_pin = 0;
const int led_pin = 2;

// System states
bool closeCondition = true;
bool openCondition = true;
int isClose = 0;
int isAuto = 1;
int rainValue = 0;
bool motorRunning = false;  

// LCD & Blynk setup
LiquidCrystal_I2C lcd(0x27, 20, 4);
BlynkTimer timer;

BLYNK_WRITE(V2) {
  isAuto = param.asInt();
  Blynk.virtualWrite(V1, isAuto);
}

BLYNK_WRITE(V0) {
  isClose = param.asInt();
  Blynk.virtualWrite(V3, isClose);
}

BLYNK_CONNECTED() {
  Serial.println("Connected to Blynk");
}

void myTimerEvent() {
  Blynk.virtualWrite(V4, rainValue);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(3000L, checkRain);  // Increased interval to avoid overlap

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Agri Shield");
  lcd.setCursor(0, 1); lcd.print("Presented by:");
  lcd.setCursor(0, 2); lcd.print("Engr. Laurente");
  lcd.setCursor(0, 3); lcd.print("Engr. Cabilin");
  delay(1500);
  lcd.clear();

  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
  pinMode(rainSensor_pin, INPUT);
  pinMode(rainPower_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkRain() {
  if (motorRunning) return;  // Prevent multiple motor commands

  if (isAuto == 0) {
    manualControl();
    return;
  }

  digitalWrite(rainPower_pin, HIGH);
  delay(10);
  rainValue = analogRead(rainSensor_pin);
  digitalWrite(rainPower_pin, LOW);
  Serial.println(rainValue);

  if (rainValue <= 800) {
    displayStatus("Raining!", "Close Roof", rainValue);
    digitalWrite(led_pin, HIGH);
    if (closeCondition) {
      Serial.println("Closing Roof");
      closeRoof();
    }
  } else {
    displayStatus("No Rain!", "Open Roof", rainValue);
    digitalWrite(led_pin, LOW);
    if (openCondition) {
      Serial.println("Opening Roof");
      openRoof();
    }
  }
}

void manualControl() {
  if (motorRunning) return;

  if (isClose == 0 && closeCondition) {
    Serial.println("Manual: Close Roof");
    displayStatus("Manual Mode", "Close Roof", rainValue);
    closeRoof();
  } else if (isClose == 1 && openCondition) {
    Serial.println("Manual: Open Roof");
    displayStatus("Manual Mode", "Open Roof", rainValue);
    openRoof();
  }
}

void openRoof() {
  motorRunning = true;
  digitalWrite(motorA, HIGH);
  digitalWrite(motorB, LOW);
  delay(2000);
  stopMotor();
  openCondition = false;
  closeCondition = true;
  motorRunning = false;
}

void closeRoof() {
  motorRunning = true;
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, HIGH);
  delay(2000);
  stopMotor();
  closeCondition = false;
  openCondition = true;
  motorRunning = false;
}

void stopMotor() {
  digitalWrite(motorA, LOW);
  digitalWrite(motorB, LOW);
}

void displayStatus(String line1, String line2, int rainVal) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(line1);
  lcd.setCursor(0, 1); lcd.print(line2);
  lcd.setCursor(0, 2); lcd.print("Rain Sensor:");
  lcd.setCursor(0, 3); lcd.print(rainVal);
}
