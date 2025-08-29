#include <esp_now.h>
#include <WiFi.h>

// Структура для получаемых данных (должна совпадать с отправителем)
typedef struct {
  char message[100];
  int counter;
  float sensor_value;
} message_struct;

message_struct receivedMessage;
bool newMessageReceived = false; // Флаг для обработки в основном цикле

// Функция обратного вызова при получении сообщения (новая версия ESP32 Core)
void onDataRecv(const esp_now_recv_info *recv_info, const uint8_t *incomingData, int len) {
  // Минимальная обработка в прерывании
  memcpy(&receivedMessage, incomingData, sizeof(receivedMessage));
  newMessageReceived = true;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // ждём подключения монитора
  }

  // Инициализация WiFi в режиме Station
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Инициализация ESP-NOW
  esp_now_init();
  
  // Регистрация функции обратного вызова для получения
  esp_now_register_recv_cb(onDataRecv);
  
}

void loop() {
  // Обработка полученных сообщений в основном цикле
  if (newMessageReceived) {
    newMessageReceived = false;
    
    Serial.println("========== ПОЛУЧЕНО СООБЩЕНИЕ ==========");
    Serial.print("Сообщение: ");
    Serial.println(receivedMessage.message);
    Serial.print("Счётчик: ");
    Serial.println(receivedMessage.counter);
    Serial.print("Значение датчика: ");
    Serial.println(receivedMessage.sensor_value);
    Serial.println("=====================================");
    Serial.println();
  }
  delay(10); // Небольшая задержка для стабильности
}