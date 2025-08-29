
## Broadcast MAC-адрес для отправки всем устройствам
```c
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
```

## Переменные для обработки ввода из консоли
```c
String inputString = "";
bool stringComplete = false;
```

## Функция обратного вызова при отправке сообщения
```c
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Статус отправки: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Успешно" : "Ошибка");
}
```

## Вывод MAC-адреса отправителя
```c
  Serial.println(WiFi.macAddress());
  ```
  
## Добавление broadcast адреса
```c
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
```
## Добавление broadcast адреса для отправки всем
```c
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  esp_now_add_peer(&peerInfo);

  Serial.println("Broadcast отправитель инициализирован");
  Serial.print("Размер структуры сообщения: ");
  Serial.print(sizeof(myMessage));
  Serial.println(" байт");
  ```

## Считывание ввода из консоли
```c
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
```
  
## Обработка введенного сообщения
```c
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
```  

## Автоматическая отправка каждые 5 секунд (только если нет активного ввода)
```c
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
  ```