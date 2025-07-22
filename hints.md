### список подключенных плат
```sh
arduino-cli board list
```
### компиляция прошивки
```sh
arduino-cli compile --fqbn esp32:esp32:esp32s2:CDCOnBoot=cdc 1_lesson
```
### загрузка прошивки на плату
```sh
arduino-cli upload --fqbn esp32:esp32:esp32s2:CDCOnBoot=cdc -p com3 1_lesson
```
### подключение к плате
```sh
arduino-cli monitor -p com3
```