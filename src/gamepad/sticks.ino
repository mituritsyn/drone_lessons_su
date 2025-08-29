#define PIN_GIMBAL_X 1
#define PIN_GIMBAL_Y 2
#define PIN_GIMBAL_RX 3
#define PIN_GIMBAL_RY 4
#define BUTTON_PIN 16

// Настройки фильтрации
#define FILTER_STRENGTH 0.3
// Переменные для тумблерной кнопки
bool lastButtonState = false;      // Предыдущее физическое состояние кнопки
bool currentButtonState = false;   // Текущее физическое состояние кнопки
bool toggleState = false;          // Текущее состояние тумблера
unsigned long lastDebounceTime = 0;// Время последнего изменения состояния
const unsigned long debounceDelay = 50; // Задержка для подавления дребезга (мс)

// Переменные для фильтрации
int16_t rawX, rawY, rawRX, rawRY;
int16_t filteredX, filteredY, filteredRX, filteredRY;
// Калибровочные значения для каждой оси {минимум, центр, максимум}
int16_t cal_x[3] = {0, 2580, 4095};   // Замерьте свои значения!
int16_t cal_y[3] = {0, 2556, 4095};
int16_t cal_rx[3] = {0, 2430, 4095};
int16_t cal_ry[3] = {0, 2400, 4095};

// Функция калибровки и масштабирования
int8_t calibrateAxis(int16_t value, int16_t min, int16_t center, int16_t max){
    // Предварительно ограничиваем входное значение
    value = constrain(value, min, max);
    int16_t result;
    if (value < center) {
        result = map(value, min, center, HID_MIN, HID_CENTER);
    } else {
        result = map(value, center, max, HID_CENTER, HID_MAX);
    }
    
    // Дополнительная защита на выходе
    return (int8_t)constrain(result, HID_MIN, HID_MAX);
}

// Простая функция фильтрации
int16_t smoothValue(int16_t new_value, int16_t old_value){
    return (old_value * FILTER_STRENGTH + new_value) / (FILTER_STRENGTH + 1);
}

void initFilteredSticksValues(){
    // Инициализация переменных фильтрации
    filteredX = analogRead(PIN_GIMBAL_X);
    filteredY = analogRead(PIN_GIMBAL_Y);
    filteredRX = analogRead(PIN_GIMBAL_RX);
    filteredRY = analogRead(PIN_GIMBAL_RY);
}

void setupSticks(){
    // Настройка АЦП
    analogReadResolution(12);
    
    // Настройка пинов
    pinMode(PIN_GIMBAL_X, INPUT);
    pinMode(PIN_GIMBAL_Y, INPUT);
    pinMode(PIN_GIMBAL_RX, INPUT);
    pinMode(PIN_GIMBAL_RY, INPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    initFilteredSticksValues();
}

void readRawSticks(){
    rawX = analogRead(PIN_GIMBAL_X);
    rawY = analogRead(PIN_GIMBAL_Y);
    rawRX = analogRead(PIN_GIMBAL_RX);
    rawRY = analogRead(PIN_GIMBAL_RY);
}

void filterSticks(){
    filteredX = (int16_t)(FILTER_STRENGTH * rawX + (1 - FILTER_STRENGTH) * filteredX);
    filteredY = (int16_t)(FILTER_STRENGTH * rawY + (1 - FILTER_STRENGTH) * filteredY);
    filteredRX = (int16_t)(FILTER_STRENGTH * rawRX + (1 - FILTER_STRENGTH) * filteredRX);
    filteredRY = (int16_t)(FILTER_STRENGTH * rawRY + (1 - FILTER_STRENGTH) * filteredRY);
}

void calibrateSticks(){
     // Калибруем каждую ось отдельно
    calibratedX = calibrateAxis(filteredX, cal_x[0], cal_x[1], cal_x[2]);
    calibratedY = calibrateAxis(filteredY, cal_y[0], cal_y[1], cal_y[2]);
    calibratedRX = calibrateAxis(filteredRX, cal_rx[0], cal_rx[1], cal_rx[2]);
    calibratedRY = calibrateAxis(filteredRY, cal_ry[0], cal_ry[1], cal_ry[2]);
}

uint32_t armState(){
    currentButtonState = !digitalRead(BUTTON_PIN);
    
     // Если состояние кнопки изменилось с отпущенного на нажатое
    if (currentButtonState != lastButtonState && currentButtonState == true) {
        toggleState = !toggleState; // Переключаем состояние тумблера
        digitalWrite(LED_BUILTIN, toggleState); // Обновляем светодиод
    }

    // Сохраняем текущее состояние для следующей итерации
    lastButtonState = currentButtonState;
    return toggleState;
}