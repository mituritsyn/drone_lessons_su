#include <esp_now.h>
#include <WiFi.h>

// Broadcast MAC-адрес для отправки всем устройствам
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#define HID_MIN -127
#define HID_CENTER 0
#define HID_MAX 127


int8_t calibratedX ,calibratedY, calibratedRX, calibratedRY;
// Структура для передаваемых данных
typedef struct{
  char message[100];
  int counter;
  float sensor_value;
} message_struct;

message_struct myMessage{"Hello there", 0, 0.0};

void setup(){
  Serial.begin(115200);
  armState();

  // Инициализация WiFi в режиме Station
  WiFi.mode(WIFI_STA);

  Serial.println(WiFi.macAddress());
  esp_now_init();

  // Добавление broadcast адреса
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Добавление peer в таблицу участников сети
  esp_now_add_peer(&peerInfo);
}

void loop(){
  esp_now_send(broadcastAddress, (uint8_t *)&myMessage, sizeof(myMessage));
  Serial.printf("Сообщение %d отправлено\n", myMessage.counter);
  myMessage.counter++;
  delay(5000); // Небольшая задержка для стабильности
}

String readSerial(){
  if (Serial.available() > 0) {
    return Serial.readStringUntil('\n'); // читаем до Enter
  }
  return "";
}

void setMessage(String newMessage){
  newMessage.trim();
  if(newMessage.length() == 0) return; // если пустая строка, не меняем
  newMessage.toCharArray(myMessage.message, sizeof(myMessage.message));
}