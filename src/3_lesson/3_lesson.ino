#include <Joystick_ESP32S2.h>

#define PIN_GIMBAL_X  1
#define LED_BUILTIN 15

Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID, 
    JOYSTICK_TYPE_MULTI_AXIS,
    4,     // 4 оси
    1,     // 1 кнопка
    true,  // X
    true,  // Y
    false, // Z
    false,  // Rx
    false,  // Ry
    false, // Rz
    true, // Rudder
    true, // Throttle
    false, // Accelerator
    false, // Brake
    false  // Steering
);

void setup() {
    Joystick.begin();
    Serial.begin(115200);
    Joystick.setXAxisRange(0, 4095);

    analogReadResolution(12);

    pinMode(PIN_GIMBAL_X, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    int16_t raw_x  = analogRead(PIN_GIMBAL_X);

    Joystick.setXAxis(raw_x);
    Serial.print("Состояние оси х: ");
    Serial.println(raw_x);

    Joystick.sendState();
    delay(100);
}