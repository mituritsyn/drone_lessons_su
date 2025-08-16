#define pin num
#define motor1 num1
#define motor2 num2
#define motor3 num3
#define motor4 num4

void setup(){
    Serial.begin(9600);
    setupIMU();
    initMotors();
    initRadio();
    Serial.println("настройка завершена");
}

void loop(){
    readIMU();
    readRadio();
    stabilize();
    spinMotor();
    
}