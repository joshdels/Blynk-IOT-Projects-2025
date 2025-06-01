//Name: Joshua S. De Leon
//Project: Blynk + DHT11 + LCD
//Date: 04/13/2024
//Comments:
/*************************************************************/

#define BLYNK_TEMPLATE_ID           "YOUR ID"
#define BLYNK_TEMPLATE_NAME         "YOUR TEMPLATE"
#define BLYNK_AUTH_TOKEN            "YOUR AUTHENTICATION"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//libraries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//dht 11 global parameters for, library <dht.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//lcd parameters
#define I2C_ADDR 0x27 // address
//pins convertion
#define SDA_PIN A0
#define SCL_PIN D3
//lcd interference
#define LCD_COLUMNS 16
#define LCD_ROWS 4
//components
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YOUR SSID";
char pass[] = "YOUR PASS";

BlynkTimer timer;

//global parameters
int temperature;
int humidity;

BLYNK_WRITE(V0)
{
  
  int value = param.asInt();
  // send writings to datastream blynk
  Blynk.virtualWrite(V1, value);
  Blynk.virtualWrite(V3, value);
}

BLYNK_CONNECTED()
{

}
void myTimerEvent()
{
 
  Blynk.virtualWrite(V2, millis() / 1000);
 
}
// crating new function and class for dht
void readDHT() 
{
  //setting value 
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  //sending data stream to blynk
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);
  
  //for adding serial monitor notes
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}
//LCD 
void LCD_ADDR()
{
  lcd.setCursor(0,0);
  lcd.print("Made by: De Leon");

  lcd.setCursor(0, 2);
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print(" °C");

  lcd.setCursor(0, 3);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.println(" %");
}

void setup()
{
  //for serial monitor
  Serial.begin(115200);
  //blynk connect
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  //dht run
  dht.begin();
  //for lcd run
  lcd.init();
  lcd.backlight();
  //timer lag for both blynk relay and dht11, and lcd
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(5000L, readDHT);
  timer.setInterval(5000L, LCD_ADDR);
}
//repeating actions
void loop()
{
  Blynk.run();
  timer.run();
}
