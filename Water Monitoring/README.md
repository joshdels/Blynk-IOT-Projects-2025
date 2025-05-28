# WaterLevelIOT
this is an esp water Level monitor of tanks
if tank reach the amount of level on the proximity sensor, it will trigger the pump to exit some water

This uses ESP8266 and Blynk to control/monitor

---------------------------------- PINS ---------------------------------------------------

Proximity
  D7 --> trigPin
  D8 --> echoPin

LCD
  D1 --> SCL
  D2 --> SDA

PUMP
  D6 --> pumpPin
