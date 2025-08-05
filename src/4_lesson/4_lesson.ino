#include <Joystick_ESP32S2.h>
#define PIN_GIMBAL_X 1
#define PIN_GIMBAL_Y 2
#define PIN_GIMBAL_RX 3
#define PIN_GIMBAL_RY 4
#define BUTTON_PIN 16
#define LED_BUILTIN 15

// Настройки фильтрации
#define FILTER_STRENGTH 3

// Калибровочные значения для каждой оси (замерьте реальные значения)
// Формат: {минимум, центр, максимум}
int16_t cal_x[3] = {0, 2580, 4095};   // Примерные значения - замерьте свои!
int16_t cal_y[3] = {0, 2550, 4095};
int16_t cal_rx[3] = {0, 2430, 4095};
int16_t cal_ry[3] = {0, 2047, 4095};

// Целевые значения для HID (обычно 0-4095 для 12-бит)
#define HID_MIN 0
#define HID_CENTER 2047
#define HID_MAX 4095

extern Joystick_ Joystick;
int16_t old_x = HID_CENTER, old_y = HID_CENTER, old_rx = HID_CENTER, old_ry = HID_CENTER;
int16_t raw_x, raw_y, raw_rx, raw_ry;
int16_t joy_x, joy_y, joy_rx, joy_ry;
long lastPrint = 0;

// Функция калибровки и масштабирования
int16_t calibrateAxis(int16_t raw_value, int16_t cal_min, int16_t cal_center, int16_t cal_max)
{
    int16_t result;
    
    if (raw_value < cal_center) {
        // Левая/нижняя половина
        result = map(raw_value, cal_min, cal_center, HID_MIN, HID_CENTER);
    } else {
        // Правая/верхняя половина  
        result = map(raw_value, cal_center, cal_max, HID_CENTER, HID_MAX);
    }
    
    // Ограничиваем значения в допустимом диапазоне
    result = constrain(result, HID_MIN, HID_MAX);
    
    return result;
}

// Простая функция фильтрации
int16_t smoothValue(int16_t new_value, int16_t old_value)
{
    return (old_value * FILTER_STRENGTH + new_value) / (FILTER_STRENGTH + 1);
}

// Функция для автоматической калибровки (вызовите один раз)
void printCalibrationValues()
{
    Serial.println("=== КАЛИБРОВКА ===");
    Serial.println("Двигайте стики во все крайние положения и в центр");
    Serial.println("Запишите минимальные, центральные и максимальные значения");
    
    for(int i = 0; i < 50; i++) {
        Serial.printf("X:%4d Y:%4d RX:%4d RY:%4d\n", 
            analogRead(PIN_GIMBAL_X), analogRead(PIN_GIMBAL_Y),
            analogRead(PIN_GIMBAL_RX), analogRead(PIN_GIMBAL_RY));
        delay(200);
    }
    Serial.println("=== КОНЕЦ КАЛИБРОВКИ ===");
}

void setup()
{
    Serial.begin(115200);
    analogReadResolution(12);
    pinMode(PIN_GIMBAL_Y, INPUT);
    pinMode(PIN_GIMBAL_RX, INPUT);
    pinMode(PIN_GIMBAL_RY, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);
    joystickSetup();
    
    // Раскомментируйте для калибровки:
    // printCalibrationValues();
}

void loop()
{
    // Читаем сырые значения
    raw_x = analogRead(PIN_GIMBAL_X);
    raw_y = analogRead(PIN_GIMBAL_Y);
    raw_rx = analogRead(PIN_GIMBAL_RX);
    raw_ry = analogRead(PIN_GIMBAL_RY);

    // Калибруем каждую ось отдельно
    int16_t cal_x_val = calibrateAxis(raw_x, cal_x[0], cal_x[1], cal_x[2]);
    int16_t cal_y_val = calibrateAxis(raw_y, cal_y[0], cal_y[1], cal_y[2]);
    int16_t cal_rx_val = calibrateAxis(raw_rx, cal_rx[0], cal_rx[1], cal_rx[2]);
    int16_t cal_ry_val = calibrateAxis(raw_ry, cal_ry[0], cal_ry[1], cal_ry[2]);

    // Применяем сглаживание к откалиброванным значениям
    joy_x = smoothValue(cal_x_val, old_x);
    joy_y = smoothValue(cal_y_val, old_y);
    joy_rx = smoothValue(cal_rx_val, old_rx);
    joy_ry = smoothValue(cal_ry_val, old_ry);

    // Сохраняем значения для следующего раза
    old_x = joy_x;
    old_y = joy_y;
    old_rx = joy_rx;
    old_ry = joy_ry;

    // Вывод в консоль
    if (millis() - lastPrint > 1000)
    {
        lastPrint = millis();
        Serial.printf("Сырые:    X:%4d Y:%4d RX:%4d RY:%4d\n", raw_x, raw_y, raw_rx, raw_ry);
        Serial.printf("Калибр:   X:%4d Y:%4d RX:%4d RY:%4d\n", cal_x_val, cal_y_val, cal_rx_val, cal_ry_val);
        Serial.printf("Финал:    X:%4d Y:%4d RX:%4d RY:%4d\n", joy_x, joy_y, joy_rx, joy_ry);
        Serial.printf("Вольтаж:  X:%4d Y:%4d RX:%4d RY:%4d\n",
                        analogReadMilliVolts(PIN_GIMBAL_X), 
                        analogReadMilliVolts(PIN_GIMBAL_Y),
                        analogReadMilliVolts(PIN_GIMBAL_RX),
                        analogReadMilliVolts(PIN_GIMBAL_RY));
        Serial.println("---");
    }

    // Отправляем откалиброванные значения
    Joystick.setXAxis(joy_x);
    Joystick.setYAxis(joy_y);
    Joystick.setThrottle(joy_rx);
    Joystick.setRudder(joy_ry);

    // Обработка кнопки
    if (!digitalRead(BUTTON_PIN))
    {
        Joystick.pressButton(0);
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
        // Joystick.releaseButton(0);
        digitalWrite(LED_BUILTIN, LOW);
    }

    Joystick.sendState();
    delay(5);
}