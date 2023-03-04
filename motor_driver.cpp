//this module handles interfacing to the motors
//current version just does pin on/off (kept separate so PWM support can be easily added)

#include "arduino.h"
#include "melty_config.h"

void motor_1_on() {
  digitalWrite(MOTOR_PIN1, HIGH);
}

void motor_2_on() {
  digitalWrite(MOTOR_PIN2, HIGH);
}

void motor_1_off() {
  digitalWrite(MOTOR_PIN1, LOW);
}

void motor_2_off() {
  digitalWrite(MOTOR_PIN2, LOW);  
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
