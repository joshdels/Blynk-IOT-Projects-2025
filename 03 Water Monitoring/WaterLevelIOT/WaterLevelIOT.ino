#include <LCD_I2C.h>
#include "secrets.h" // delete this if you manually insert the credentials
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

  float distance = 0;
  int value = 0;

// Pins
  const int pumpPin = 12;
  const int trigPin = 13;
  const int echoPin = 15;


// LCD and Blynk Settings
  LCD_I2C lcd(0x27, 16, 2);
  BlynkTimer timer;

  BLYNK_WRITE(V0) {
    value = param.asInt();
    Blynk.virtualWrite(V1, value);
  }
    
  BLYNK_CONNECTED() {
    // This will run if the blynk is connected
    Serial.println("Connected to Blink");
  }

  void myTimerEvent(){
    Blynk.virtualWrite(V2, millis() / 1000);
    Blynk.virtualWrite(V4, distance);
  }

float sonarReading() {
  //this function is for the LCD reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2;

  return distance;  
}

void setup() {
  Serial.begin(115200);
  // Pump
  pinMode(pumpPin, OUTPUT);
  // Sonar
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // LCD
  lcd.begin();
  lcd.backlight();

  //Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  // Blynk Settings
  Blynk.run();
  timer.run();

  distance = sonarReading();

  delay(1000);

  Serial.print("Dist: ");
  Serial.print(distance);
  Serial.println(" cm"); 
  
  // LCD
  lcd.clear();
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm"); 

  delay(1000);

  // Main Logic
  if (value == 1) {
    // when blynk ON buttons turns on
    digitalWrite(pumpPin, HIGH);
    Serial.println("ON PUMP");
    lcd.setCursor(0, 1);
    lcd.print("Pumping Water");
  } else {
    
    if (distance <= 5) {
      // if it hit the certain number, it activates the pump
      digitalWrite(pumpPin, HIGH);
      Serial.print("Pumping Water");

      lcd.setCursor(0, 1);
      lcd.print("Pumping Water");

    } else {
      digitalWrite(pumpPin, LOW);
    }
  };


delay(1000);
}






