## Включение библиотеки
```C
#include <Joystick_ESP32S2.h>
```
## Создание объекта Joystick
```C
Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID, // номер нашего джойстика,
    JOYSTICK_TYPE_MULTI_AXIS, // сообщаем системе, что это многоосевой джойстик
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
)
```
## Запуск работы джойстика 
```C
Joystick.begin();
```
## Настраиваем диапазон значений, передаваемых в джойстик
```C
Joystick.setXAxisRange(0, 4095);
```
## Передаем значение оси в объект Joystick
```C
Joystick.setXAxis(joy_x);
```
## Отправляем данные на компьютер
```C
Joystick.sendState();
```
