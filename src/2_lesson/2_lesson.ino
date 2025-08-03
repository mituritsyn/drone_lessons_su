#define MOTOR_PIN 1
#define LED_PIN 15
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {
  for(int i=0; i<=255; i++){
    analogWrite(LED_PIN, i);
    analogWrite(MOTOR_PIN, i);
    delay(15);
  }
  for(int i=255; i>=0; i--){
    analogWrite(LED_PIN, i);
    analogWrite(MOTOR_PIN, i);
    delay(15);
  }
  delay(100);
}