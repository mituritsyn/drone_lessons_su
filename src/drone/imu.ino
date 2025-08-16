#include <FlixPeriph.h>
#include <SPI.h>
#include <ICM20948.h>

ICM20948 IMU(SPI);
float ax, ay, az;

// Функция для инициализации IMU
void setupIMU(){
    //Serial.begin(115200);
    IMU.begin();
    configureIMU();
    Serial.printf("ID устройства IMU: 0x%02X\n", IMU.whoAmI()); // Выводим ID устройства IMU в Serial Monitor
}

// Функция для настройки IMU
void configureIMU(){
    IMU.setAccelRange(IMU.ACCEL_RANGE_4G);
    IMU.setGyroRange(IMU.GYRO_RANGE_2000DPS);
    IMU.setDLPF(IMU.DLPF_MAX);
    IMU.setRate(IMU.RATE_1KHZ_APPROX);
    IMU.setupInterrupt();
}

// Функция для чтения данных с IMU
void readIMU(){
    IMU.waitForData();
    IMU.getAccel(ax, ay, az);
}

// Функция для преобразования данных IMU в значения джойстика
void transformIMUData(int8_t& joyX, int8_t& joyY) {
   // Переводим в углы (в градусах)
    float angleX = atan2(ay, az) * 180.0 / PI;
    float angleY = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

    joyX = map(angleX, -60, 60, -127, 127);
    joyY = map(angleY, -60, 60, -127, 127);
}