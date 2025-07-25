#define LED_PIN 15
#define BUTTON_PIN 0
#define X_PIN 1
int x = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  pinMode(X_PIN, INPUT);
}

void loop() {
  Serial.println("Привет, мир!");
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  if (digitalRead(BUTTON_PIN)) {
    Serial.println("Кнопка не нажата!");
  } else {
    Serial.println("Кнопка нажата.");
  }
  x = analogRead(X_PIN);  // читаем ось x
  Serial.println(x);
}