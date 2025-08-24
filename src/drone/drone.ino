
void setup(){
    Serial.begin(9600);
    setupIMU();
    setupMotors();
    initRadio();
    Serial.println("настройка завершена");
}

void loop(){
    readIMU();
    readRadio();
    stabilize();
    spinMotor();
    
}