//this module handles interfacing to the motors
//current version just does pin on/off (kept separate so PWM support can be easily added)

#include "arduino.h"
#include "melty_config.h"

float get_battery_voltage() {
  return (analogRead(BATTERY_ADC_PIN) * (ARDUINIO_VOLTAGE / 1023.0)) * VOLTAGE_DIVIDER;
}

//check for low battery - but only alarm after certain number of low reads in a row (prevents ADC noise from alarming)
int battery_voltage_low(){
  static int low_bat_count = 0;
  if (get_battery_voltage() < BATTERY_ADC_WARN_VOLTAGE_THRESHOLD) {
    low_bat_count++;
  } else {
    low_bat_count = 0;
  }
  if (low_bat_count > LOW_BAT_REPEAT_READS_BEFORE_ALARM) return 1;
  return 0;
}
