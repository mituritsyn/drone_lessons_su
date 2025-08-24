#define MOTORS_COUNT 4
#define PWM_RESOLUTION 12
#define PWM_FREQUENCY 80000000/2^PWM_RESOLUTION
// 1<<PWM_RESOLUTION

int motorsPower[MOTORS_COUNT];
int motorPins[] = {1,2,3,4};

void setupMotors(){
    for(int i=0; i < MOTORS_COUNT; i++){
        // pinMode
        ledcAttach(motorPins[i], PWM_FREQUENCY, PWM_RESOLUTION);
    }
}
 void spinMotor(){
    for(int i=0; i < MOTORS_COUNT; i++){
        // analogWrite
        ledcWrite(motorPins[i], motorsPower[i]);
    }
 }