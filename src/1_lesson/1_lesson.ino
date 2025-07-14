#include <Arduino.h>

#define LED_BUILTIN 15
#define BOOT_BUTTON 0

unsigned long prevMillis = 0;
const uint16_t interval = 500;
bool ledState = false;

void setup(){
  pinMode(BOOT_BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Hello, World!");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
}

void loop(){
  Serial.println("loop again");
  if (digitalRead(BOOT_BUTTON) == LOW) {
    Serial.println("Boot button pressed!");
  }
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
  }
}