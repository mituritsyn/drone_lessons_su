### инициализация(начальная настройка) ардуино фреймворка и создание YAML файла конфигурации
```
arduino-cli config init
```
### обновление информации о доступных библиотеках и ядрах
```
arduino-cli core update-index
```
### установка ядра esp32
```
arduino-cli core install esp32:esp32
```
### если установка ядра закончилась ошибкой Timeout
```
arduino-cli config set network.connection_timeout 600s
```

### информация о плате по fqbn
```
arduino-cli board details --fqbn esp32:esp32:esp32s3
```