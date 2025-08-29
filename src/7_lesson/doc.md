
## Broadcast MAC-адрес для отправки всем устройствам
```c
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
```

## Инициализация WiFi и esp_now
```c
WiFi.mode(WIFI_STA);
esp_now_init();
```

## Функция callback (обратного вызова) при отправке сообщения
```c
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Статус отправки: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Успешно" : "Ошибка");
}
// Регистрация callback функции
esp_now_register_send_cb(onDataSend);
```

## Вывод MAC-адреса
```c
  Serial.println(WiFi.macAddress());
```
  
## Формирование структуры с информацией об участниках сети
```c
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
```

## Добавление broadcast адреса для отправки всем
```c
  esp_now_add_peer(&peerInfo);
```

