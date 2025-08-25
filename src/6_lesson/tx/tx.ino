#include <esp_now.h>
#include <WiFi.h>

// Broadcast MAC-адрес для отправки всем устройствам
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Структура для передаваемых данных
typedef struct {
  char message[100];
  int counter;
  float sensor_value;
} message_struct;

message_struct myMessage;
int messageCounter = 0;

// Функция обратного вызова при отправке сообщения
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Статус отправки на ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(": ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Успешно" : "Ошибка");
}

void setup() {
  Serial.begin(115200);
  
  // Инициализация WiFi в режиме Station
  WiFi.mode(WIFI_STA);
  
  // Вывод MAC-адреса отправителя
  Serial.print("MAC-адрес отправителя: ");
  Serial.println(WiFi.macAddress());
  
  // Инициализация ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Ошибка инициализации ESP-NOW");
    return;
  }
  
  // Регистрация функции обратного вызова для отправки
  esp_now_register_send_cb(OnDataSent);
  
  // Добавление broadcast адреса
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  // Добавление broadcast адреса для отправки всем
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Не удалось добавить broadcast адрес");
  } else {
    Serial.println("Broadcast адрес добавлен успешно");
  }
  
  Serial.println("Broadcast отправитель инициализирован");
}

void loop() {
  // Подготовка сообщения
  messageCounter++;
  sprintf(myMessage.message, "Привет от ESP32! Сообщение #%d", messageCounter);
  myMessage.counter = messageCounter;
  myMessage.sensor_value = random(0, 1000) / 10.0; // Имитация показаний датчика
  
  // Отправка broadcast сообщения
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
  
  if (result == ESP_OK) {
    Serial.println("Broadcast сообщение отправлено успешно");
  } else {
    Serial.println("Ошибка отправки broadcast сообщения");
  }
  
  delay(5000); // Отправка каждые 5 секунд
}