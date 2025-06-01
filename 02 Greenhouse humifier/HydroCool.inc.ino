/************************************************************

 HydroCool.inc  est. 6/6/2024
 specifically for precision engineering
 Rey De leon

    Made by: 
  Joshua S. De Leon
  Henster S. Legara
  Al Kristian Lou V. Presente

*************************************************************/
#define BLYNK_TEMPLATE_ID           "YOUR ID"
#define BLYNK_TEMPLATE_NAME         "YOUR TEMPLATE"
#define BLYNK_AUTH_TOKEN            "YOUR AUTH"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// Libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// DHT11 global parameters
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Fan and humidifier
const int fanPin = D5;
const int humidifierPin = D6;

// LCD parameters
LiquidCrystal_I2C lcd(0x27, 16, 4); // Adjusted LCD dimensions

// Your WiFi credentials
char ssid[] = "YOUR SSID";
char pass[] = "YOUR PASS";

BlynkTimer timer;

// Global parameters
float temperature;
float humidity;
int switchState;

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  switchState = param.asInt();
  Blynk.virtualWrite(V1, switchState);
}

BLYNK_WRITE(V1)
{
  int value = param.asInt();
  switchState = value;
}

void myTimerEvent()
{
  Blynk.virtualWrite(V2, millis() / 1000);
}

void readDHT()
{
  // Reading temperature and humidity
  float newTemperature = dht.readTemperature();
  float newHumidity = dht.readHumidity();

  temperature = newTemperature;
  humidity = newHumidity;

  // Sending data to Blynk
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);

  // Displaying on Serial Monitor
  Serial.print("Temp: ");
  Serial.print(newTemperature);
  Serial.print(" °C\tHumi: ");
  Serial.print(newHumidity);
  Serial.println(" %");

  Serial.print("switchState: ");
  Serial.println(switchState);
}

void displayLCD()
{
  lcd.clear(); // Clear LCD before updating
  lcd.setCursor(0, 0);
  lcd.print("HydroCool inc.");

  lcd.setCursor(0, 2); // Adjusted row index
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 3); // Adjusted row index
  lcd.print("Humi: ");
  lcd.print(humidity);
  lcd.print(" %"); 
}

void setup()
{
  // Initializing sensors
  pinMode(fanPin, OUTPUT);
  pinMode(humidifierPin, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  lcd.init();
  lcd.backlight();

  // Setting up timers
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(1000L, readDHT);
  timer.setInterval(1000L, displayLCD);
}

void loop() {
  delay(2000);
  Blynk.run();
  timer.run();


  // Controlling fan and humidifier based on readings and switch state
  //normally open hardware switch, thats why inverse high/low values
  if (temperature > 30.0 && switchState == HIGH) {
    digitalWrite(fanPin, LOW);
  } else {
    digitalWrite(fanPin, HIGH);
  }

  if  (humidity < 60.0 && switchState == HIGH) {
    digitalWrite(humidifierPin, LOW);
  } else {
    digitalWrite(humidifierPin, HIGH);
  }

  if (switchState == LOW) { // Fixed syntax error here
    digitalWrite(fanPin, HIGH);
    digitalWrite(humidifierPin, HIGH); // Corrected capitalization of 'humidifierPin'
  }
}
