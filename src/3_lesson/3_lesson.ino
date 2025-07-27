#define PIN_GIMBAL_X  1
#define LED_BUILTIN 15

void setup() {
    Serial.begin(115200);

    analogReadResolution(12);

    pinMode(PIN_GIMBAL_X, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    int16_t raw_x  = analogRead(PIN_GIMBAL_X);

    Serial.print("Состояние оси х: ");
    Serial.println(raw_x);

    delay(100);
}