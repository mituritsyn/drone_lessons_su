#define LED_BUILTIN 15
#define BOOT_BUTTON 0

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
  digitalWrite(LED_BUILTIN, LOW);   // Выключить LED
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);  // Включить LED
  delay(500);
}