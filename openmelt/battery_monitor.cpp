
#include "arduino.h"
#include "melty_config.h"

//returns battery voltage
float get_battery_voltage() {
  return (analogRead(BATTERY_ADC_PIN) * (ARDUINIO_VOLTAGE / 1023.0)) * VOLTAGE_DIVIDER;
}

//check for low battery - but only alarm after certain number of low reads in a row (prevents ADC noise from alarming)
//(settings in melty_config.h)
int battery_voltage_low(){
  static int low_bat_count = 0;
  if (get_battery_voltage() < BATTERY_ADC_WARN_VOLTAGE_THRESHOLD) {
    low_bat_count++;
  } else {
    low_bat_count = 0;
  }
  if (low_bat_count > LOW_BAT_REPEAT_READS_BEFORE_ALARM) return true;
  return false;
}

