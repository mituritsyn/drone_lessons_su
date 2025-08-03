

Joystick_ Joystick(
    JOYSTICK_DEFAULT_REPORT_ID,
    JOYSTICK_TYPE_MULTI_AXIS,
    4,     // 4 оси
    1,     // 1 кнопка
    true,  // X
    true,  // Y
    false, // Z
    false, // Rx
    false, // Ry
    false, // Rz
    true,  // Rudder
    true,  // Throttle
    false, // Accelerator
    false, // Brake
    false  // Steering
);

void joystickSetup(){
    Joystick.begin();
    Joystick.setXAxisRange(0, 4095);
    Joystick.setYAxisRange(0, 4095);
    Joystick.setThrottleRange(0, 4095);
    Joystick.setRudderRange(0, 4095);
}