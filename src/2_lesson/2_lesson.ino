#define MOTOR_PIN 1
#define LED_BUILTIN 15

void setup(){
 pinMode(MOTOR_PIN, OUTPUT);
 pinMode(LED_BUILTIN, OUTPUT);
 Serial.begin(115200);
}
void loop(){
 for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
   analogWrite(MOTOR_PIN, dutyCycle);
   analogWrite(LED_BUILTIN, dutyCycle);
   delay(15);
 }
 for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
   analogWrite(MOTOR_PIN, dutyCycle);
   analogWrite(LED_BUILTIN, dutyCycle);
   delay(15);
 }
}
