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

// Переменные для обработки ввода из консоли
String inputString = "";
bool stringComplete = false;

// Функция обратного вызова при отправке сообщения
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Статус отправки на ");
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(": ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Успешно" : "Ошибка");
}

void setup() {
  Serial.begin(115200);
  
  // Инициализация WiFi в режиме Station
  WiFi.mode(WIFI_STA);
  
  // Вывод MAC-адреса отправителя
  Serial.println("=== ESP32 ESP-NOW Отправитель ===");
  Serial.print("MAC-адрес отправителя: ");
  Serial.println(WiFi.macAddress());
  
esp_now_init() != ESP_OK) {

  // Регистрация функции обратного вызова для отправки
  esp_now_register_send_cb(OnDataSent);
  
  // Добавление broadcast адреса
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  // Добавление broadcast адреса для отправки всем
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  esp_now_add_peer(&peerInfo);

  Serial.println("Broadcast отправитель инициализирован");
  Serial.print("Размер структуры сообщения: ");
  Serial.print(sizeof(myMessage));
  Serial.println(" байт");
  
  // Инструкции пользователю
  Serial.println("===============================");
  Serial.println("Введите сообщение и нажмите Enter для отправки");
  Serial.println("Или оставьте пустым для автоматической отправки каждые 5 сек");
  Serial.print("> ");
}

void loop() {
  // Считывание ввода из консоли
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
  
  // Обработка введенного сообщения
  if (stringComplete) {
    messageCounter++;
    
    if (inputString.length() > 0) {
      // Отправляем введенное пользователем сообщение
      inputString.toCharArray(myMessage.message, sizeof(myMessage.message));
      Serial.println("Отправляю введенное сообщение: " + inputString);
    } else {
      // Отправляем стандартное сообщение, если ничего не введено
      sprintf(myMessage.message, "Привет от ESP32! Сообщение #%d", messageCounter);
      Serial.println("Отправляю стандартное сообщение");
    }
    
    myMessage.counter = messageCounter;
    myMessage.sensor_value = random(0, 1000) / 10.0;
    
    // Отправка broadcast сообщения
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
    
    if (result == ESP_OK) {
      Serial.println("✓ Broadcast сообщение отправлено успешно");
    } else {
      Serial.println("✗ Ошибка отправки broadcast сообщения");
    }
    
    // Сброс переменных для следующего ввода
    inputString = "";
    stringComplete = false;
    Serial.println("---");
    Serial.print("> ");
  }
  
  // Автоматическая отправка каждые 5 секунд (только если нет активного ввода)
  static unsigned long lastAutoSend = 0;
  if (millis() - lastAutoSend > 5000 && !stringComplete && inputString.length() == 0) {
    messageCounter++;
    sprintf(myMessage.message, "Авто-сообщение #%d", messageCounter);
    myMessage.counter = messageCounter;
    myMessage.sensor_value = random(0, 1000) / 10.0;
    
    Serial.println("Отправляю автоматическое сообщение");
    
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myMessage, sizeof(myMessage));
    
    if (result == ESP_OK) {
      Serial.println("✓ Авто-сообщение отправлено");
    } else {
      Serial.println("✗ Ошибка отправки авто-сообщения");
    }
    
    lastAutoSend = millis();
    Serial.println("---");
    Serial.print("> ");
  }
  
  delay(10); // Небольшая задержка для стабильности
}