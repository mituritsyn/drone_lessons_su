# IMU - Inertial Measurement Unit, датчик для определения ориентации и движения объекта в пространстве.

Для работы с ним необходимо:

## Подключение библиотек
```C
#include <FlixPeriph.h>
#include <SPI.h>
#include <ICM20948.h>
```

## Запуск IMU
```C
IMU.begin();
```

## Вывод статуса IMU

```C
Serial.printf("ID устройства IMU: 0x%02X\n", IMU.whoAmI());
```
Если датчик корректно подключен и работает, должен получиться код 0xEA

## Настройки IMU
```C
IMU.setAccelRange(IMU.ACCEL_RANGE_4G);
IMU.setGyroRange(IMU.GYRO_RANGE_2000DPS);
IMU.setDLPF(IMU.DLPF_MAX);
IMU.setRate(IMU.RATE_1KHZ_APPROX);
IMU.setupInterrupt();
```

## Ожидание данных IMU
```C
IMU.waitForData();
```
## Получение значений по осям акселерометра
```C
IMU.getAccel(ax, ay, az);
```

## Функция для преобразования данных IMU
```C
void transformIMUData(float ax, float ay, float az, int8_t& joyX, int8_t& joyY) {
   // Переводим в углы (в градусах)
    float angleX = atan2(ay, az) * 180.0 / PI;
    float angleY = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

    joyX = map(angleX, -60, 60, -127, 127);
    joyY = map(angleY, -60, 60, -127, 127);
}
```
