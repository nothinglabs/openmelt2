//intitialize motors
void init_motors();

//turn motor_X_on (throttle_percent only used for dynamic PWM throttle mode)
void motor_1_on(float throttle_percent);
void motor_2_on(float throttle_percent);

//motors shut-down (robot not translating)
void motor_1_off();
void motor_2_off();
void motors_off();

//motors coasting (unpowered part of rotation when translating)
void motor_1_coast();
void motor_2_coast();

