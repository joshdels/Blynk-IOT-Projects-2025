#include "secrets.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define BLYNK_PRINT Serial

// ----------------------------------------------------------------------------------------------------

BlynkTimer timer;

BLYNK_WRITE(V0) {
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);
}

BLYNK_CONNECTED() {
  // This will run if the blynk is connected
  Serial.println("Connected to Blink");
}

void myTimerEvent(){
  Blynk.virtualWrite(V2, millis() / 1000);
}


// -----------------------------------------


void setup() {
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);

}

void loop() {
  Blynk.run();
  timer.run();

}

