#define LED_BUILTIN 15

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  Serial.begin(115200);
}

void loop() {
  if (digitalRead(0) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
    dacWrite(17, 255);  // Устанавливаем максимальное значение на DAC
  } else {
    Serial.println("Кнопка не нажата");
    digitalWrite(LED_BUILTIN, LOW);
    dacWrite(17, 0);  // Устанавливаем минимальное значение на DAC
  }
  delay(100);
  //   for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
  //     // меняем яркость светодиода при помощи ШИМ:
  //     ledcWrite(LED_BUILTIN, dutyCycle);
  //     analogWrite(LED_BUILTIN, dutyCycle);
  //     delay(15);
  //   }

  //   // уменьшаем яркость светодиода:
  //   for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
  //     // меняем яркость светодиода при помощи ШИМ:
  //     ledcWrite(LED_BUILTIN, dutyCycle);
  //     analogWrite(LED_BUILTIN, dutyCycle);
  //     delay(15);
  //   }
}