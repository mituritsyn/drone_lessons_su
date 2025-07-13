#define LED_BUILTIN 15

void setup(){
  Serial.begin(115200);
  Serial.println("Hello, World!");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000);
}

void loop(){
}