#include <Joystick_ESP32S2.h>

#define PIN_GIMBAL_X  1
#define PIN_GIMBAL_Y  2
#define PIN_GIMBAL_RX 3
#define PIN_GIMBAL_RY 4
#define BUTTON_PIN    0
#define LED_BUILTIN   15

// Настройки фильтрации (чем больше число - тем больше сглаживание)
#define FILTER_STRENGTH 10   // от 1 до 10
#define CENTER_VALUE    2590 // центр для 12-битного АЦП

// Переменные для хранения предыдущих значений
int16_t old_x = CENTER_VALUE, old_y = CENTER_VALUE, old_rx = CENTER_VALUE, old_ry = CENTER_VALUE;
// Переменные для текущих значений (объявляем один раз)
int16_t raw_x, raw_y, raw_rx, raw_ry;
int16_t joy_x, joy_y, joy_rx, joy_ry;
long lastPrint = 0;

Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID,
    JOYSTICK_TYPE_MULTI_AXIS,
    4,     // 4 оси
    1,     // 1 кнопка
    true,  // X
    true,  // Y
    false, // Z
    false, // Rx
    false, // Ry
    false, // Rz
    true,  // Rudder
    true,  // Throttle
    false, // Accelerator
    false, // Brake
    false  // Steering
);

// Простая функция фильтрации - сглаживает резкие скачки
int16_t smoothValue(int16_t new_value, int16_t old_value) {
    // Простое среднее арифметическое между старым и новым значением
    return (old_value * FILTER_STRENGTH + new_value) / (FILTER_STRENGTH + 1);
}

void setup() {
    Serial.begin(115200);    analogReadResolution(12);

    pinMode(PIN_GIMBAL_X, INPUT);
    pinMode(PIN_GIMBAL_Y, INPUT);
    pinMode(PIN_GIMBAL_RX, INPUT);
    pinMode(PIN_GIMBAL_RY, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    
    digitalWrite(LED_BUILTIN, LOW);

    Joystick.begin();
    Joystick.setXAxisRange(0, 4095);
    Joystick.setYAxisRange(0, 4095);
    Joystick.setThrottleRange(0, 4095);
    Joystick.setRudderRange(0, 4095);
}

void loop() {
    // Читаем сырые значения с аналоговых пинов
    raw_x  = analogRead(PIN_GIMBAL_X);
    raw_y  = analogRead(PIN_GIMBAL_Y);
    raw_rx = analogRead(PIN_GIMBAL_RX);
    raw_ry = analogRead(PIN_GIMBAL_RY);
    
    // Применяем сглаживание (убираем резкие скачки)
    joy_x  = smoothValue(raw_x, old_x);
    joy_y  = smoothValue(raw_y, old_y);
    joy_rx = smoothValue(raw_rx, old_rx);
    joy_ry = smoothValue(raw_ry, old_ry);
    
    // Сохраняем значения для следующего раза
    old_x = joy_x;
    old_y = joy_y;
    old_rx = joy_rx;
    old_ry = joy_ry;

    // Вывод в консоль раз в секунду
    if (millis() - lastPrint > 1000) {
        lastPrint = millis();
        Serial.printf("Сырые: X:%4d Y:%4d RX:%4d RY:%4d\n", raw_x, raw_y, raw_rx, raw_ry);
        Serial.printf("Фильтр: X:%4d Y:%4d RX:%4d RY:%4d\n", joy_x, joy_y, joy_rx, joy_ry);
        Serial.println("---");
    }

    // Отправляем отфильтрованные значения в джойстик
    Joystick.setXAxis(joy_x);
    Joystick.setYAxis(joy_y);
    Joystick.setThrottle(joy_rx);
    Joystick.setRudder(joy_ry);

    // Обработка кнопки
    if (!digitalRead(BUTTON_PIN)) {
        Joystick.pressButton(0);
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        Joystick.releaseButton(0);
        digitalWrite(LED_BUILTIN, LOW);
    }

    Joystick.sendState();
    delay(10);
}