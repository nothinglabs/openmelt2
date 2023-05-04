//this module handles interfacing to the motors
//current version just does pin on/off (kept separate so PWM support can be easily added)

#include "arduino.h"
#include "melty_config.h"

//motor_X_on functions are used for the powered phase of each rotation
//motor_X_coast functions are used for the unpowered phase of each rotation
//motor_X_off functions are used for when the robot is spun-down

void motor_1_on() {
#ifdef ENABLE_PWM_MOTOR_CONTROL
  analogWrite(MOTOR_PIN1, PWM_MOTOR_ON);
#else
  digitalWrite(MOTOR_PIN1, HIGH);
#endif
}

void motor_2_on() {
#ifdef ENABLE_PWM_MOTOR_CONTROL
  analogWrite(MOTOR_PIN2, PWM_MOTOR_ON);
#else
  digitalWrite(MOTOR_PIN2, HIGH);
#endif
}

void motor_1_coast() {
#ifdef ENABLE_PWM_MOTOR_CONTROL
  analogWrite(MOTOR_PIN1, PWM_MOTOR_COAST);
#else
  digitalWrite(MOTOR_PIN1, LOW);  //same as "off" for brushed motors
#endif
}

void motor_2_coast() {
#ifdef ENABLE_PWM_MOTOR_CONTROL
  analogWrite(MOTOR_PIN2, PWM_MOTOR_COAST);
#else
  digitalWrite(MOTOR_PIN2, LOW);  //same as "off" for brushed motors
#endif
}

void motor_1_off() {
#ifdef ENABLE_PWM_MOTOR_CONTROL
  analogWrite(MOTOR_PIN1, PWM_MOTOR_OFF);
#else
  digitalWrite(MOTOR_PIN1, LOW);
#endif
}

void motor_2_off() {
#ifdef ENABLE_PWM_MOTOR_CONTROL
  analogWrite(MOTOR_PIN2, PWM_MOTOR_OFF);
#else
  digitalWrite(MOTOR_PIN2, LOW);
#endif
}

void motors_off() {
  motor_1_off();
  motor_2_off();
}

void init_motors() {
  pinMode(MOTOR_PIN1, OUTPUT);
  pinMode(MOTOR_PIN2, OUTPUT);
  motors_off();
}
