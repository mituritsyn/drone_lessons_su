#include "USB.h"
#include "USBHIDGamepad.h"

#define LED_BUILTIN 15

// Диапазон для HID геймпада
#define HID_MIN -127
#define HID_CENTER 0
#define HID_MAX 127

// Создаем объект геймпада
USBHIDGamepad gamepad;

int8_t calibratedX ,calibratedY, calibratedRX, calibratedRY;

void setup(){
    Serial.begin(115200);
    setupSticks();
    digitalWrite(LED_BUILTIN, LOW);
    // Инициализация USB HID и геймпада
    USB.begin();
    gamepad.begin();
    Serial.println("USB HID Gamepad готов!");
}

void loop(){
    // Читаем сырые значения
    readRawSticks();
    // Применяем сглаживание 
    filterSticks();
    // Применяем калибровку центральных значений
    calibrateSticks();
    // Отправляем состояние геймпада
    gamepad.send(0,0,calibratedX, calibratedY, calibratedRX, calibratedRY, 0, armState());
    delay(10);
}