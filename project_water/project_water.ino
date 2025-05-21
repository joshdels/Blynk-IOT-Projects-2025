#include <LCD_I2C.h>

// Pump
  const int pumpPin = 12;

//Sonar
  const int trigPin = 13;
  const int echoPin = 15;
  long distance = 0;

// LCD
  LCD_I2C lcd(0x27, 16, 2);

long sonarReading() {
  //this function is for the LCD reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.0344 / 2;

  return distance;
  
}

void setup() {
  Serial.begin(115200);
  // Pump
    digitalWrite(pumpPin, OUTPUT);
  // LCD
    lcd.begin();
    lcd.backlight();
  // Sonar
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
  long distance = sonarReading();

  delay(2000);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm"); 
  
  // LCD
  lcd.clear();
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm"); 

  delay(1000);

  if (distance <= 20) {
    // if it hit the certain number, it activates the pump
    digitalWrite(pumpPin, HIGH);
    delay(1000);
    digitalWrite(pumpPin, LOW);

    Serial.print("Pumping Water");
    lcd.setCursor(0, 1);
    lcd.print("Pumping Water");
  }

delay(1000);
}






