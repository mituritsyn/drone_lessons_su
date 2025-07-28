#include <Joystick_ESP32S2.h>

#define PIN_GIMBAL_X  1
#define PIN_GIMBAL_Y  2
#define LED_BUILTIN 15

int16_t raw_x = 0;
int16_t raw_y = 0;

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
    pinMode(PIN_GIMBAL_Y, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    raw_x  = analogRead(PIN_GIMBAL_X);
    raw_y  = analogRead(PIN_GIMBAL_Y);

    Joystick.setXAxis(raw_x);
    Joystick.setYAxis(raw_y);
    Serial.print("Состояние оси х: ");
    Serial.println(raw_x);

    Joystick.sendState();
    delay(100);
}