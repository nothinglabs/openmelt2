//this module handles calculation and timing loop for translational drift
//also interfaces directly with the heading LED

#include "arduino.h"
#include "melty_config.h"
#include "motor_driver.h"
#include "rc_handler.h"
#include "spin_control.h"
#include "accel_handler.h"

static const struct device *dev;

void init_melty(void) {
  pinMode(HEADING_LED_PIN, OUTPUT);
}

void heading_led_on() {
  digitalWrite(HEADING_LED_PIN, HIGH);
}

void heading_led_off() {
  digitalWrite(HEADING_LED_PIN, LOW);
}


//calculates time for this rotation of robot
//is increased / decreased by a factor relative to RC left / right position
//by having the rotation time intentionally run slighty short or long - heading of robot is effectively changed
static float get_rotation_interval_ms(void) {

  //increasing causes tracking speed to decrease
  float radius_in_cm = ACCEL_MOUNT_RADIUS_CM;
  
  //TODO: implement this... removed for testing
  //This approach didn't work as expected - may be a bug - or maybe just need to change constants
  //float radius_adjustment_factor = (rc_get_leftright() / NOMINAL_PULSE_RANGE) / LEFT_RIGHT_HEADING_CONTROL_DIVISOR;
  //radius_in_cm = radius_in_cm + (radius_in_cm * radius_adjustment_factor);

  //calculate RPM from g's - derived from "G = 0.00001118 * r * RPM^2"
  float rpm;
  rpm = get_accel_force_g() * 89445.0f;
  rpm = rpm / radius_in_cm;
  rpm = sqrt(rpm);

  float rotation_interval = (1.0f / rpm) * 60 * 1000;
  if (rotation_interval > 250) rotation_interval = 250;
  if (rotation_interval < 0) rotation_interval = 250;
  return rotation_interval;
}

//Calculates all parameters need for a single rotation
//Motor timing, LED timing, etc.
//This entire section takes ~1300us on an Atmega32u4 (acceptable)
static struct melty_parameters_t get_melty_parameters(void) {

  float led_offset_portion = LED_OFFSET_PERCENT / 100.0f;
  float motor_on_portion = rc_get_throttle_percent() / 100.0f;
  float led_on_portion = .4f * (1.1f - motor_on_portion);  //LED width changed with throttle

  struct melty_parameters_t melty_parameters;

  melty_parameters.translate_forback = rc_get_forback();
  
  //TODO: THIS IS JUST TESTING!
  melty_parameters.rotation_interval_us = get_rotation_interval_ms() * 1000;
  melty_parameters.rotation_interval_us = melty_parameters.rotation_interval_us + rc_get_leftright() * 80;

  //if under defined RPM - just try to spin up
  if (melty_parameters.rotation_interval_us > MAX_TRANSLATION_ROTATION_INTERVAL_US) motor_on_portion = 1;

  //if we are too slow - don't even try to track heading
  if (melty_parameters.rotation_interval_us > MAX_TRACKING_ROTATION_INTERVAL_US) {
    melty_parameters.rotation_interval_us = MAX_TRACKING_ROTATION_INTERVAL_US;
  }

  unsigned long motor_on_us = motor_on_portion * melty_parameters.rotation_interval_us;

  unsigned long led_on_us = led_on_portion * melty_parameters.rotation_interval_us;
  unsigned long led_offset_us = led_offset_portion * melty_parameters.rotation_interval_us;

  //center LED on offset
  if (led_on_us / 2 <= led_offset_us) {
    melty_parameters.led_start = led_offset_us - (led_on_us / 2);
  } else {
    melty_parameters.led_start = (melty_parameters.rotation_interval_us + led_offset_us) - (led_on_us / 2);
  }

  melty_parameters.led_stop = melty_parameters.led_start + led_on_us;

  if (melty_parameters.led_stop > melty_parameters.rotation_interval_us)
    melty_parameters.led_stop = melty_parameters.led_stop - melty_parameters.rotation_interval_us;

  melty_parameters.motor_start1 = (melty_parameters.rotation_interval_us - motor_on_us) / 2;
  melty_parameters.motor_stop1 = melty_parameters.motor_start1 + motor_on_us;

  melty_parameters.motor_start2 = melty_parameters.rotation_interval_us - (motor_on_us / 2);
  melty_parameters.motor_stop2 = motor_on_us / 2;

  return melty_parameters;
}

//rotates the robot once + handles translational drift
//(repeat as needed)
void spin_one_rotation(void) {

  //cycle count is tracked to help handle idling (non-translation)
  //overflow is non-issue
  static unsigned long cycle_count = 0;
  cycle_count++;

  //capture initial time stamp
  //by starting before the loop - time performing accel sampling / floating point math is included in loop time
  unsigned long start_time = micros();

  lock_rc_data();   //prevent changes to RC values during calculations
  //all slow stuff (floating point math / sensor reads) should happen here...
  struct melty_parameters_t melty_parameters = get_melty_parameters();
  unlock_rc_data();
  
  unsigned long time_spent_this_rotation_us = 0;
  //loop for one rotation of robot
  while (time_spent_this_rotation_us < melty_parameters.rotation_interval_us) {

    //If translation direction is RC_FORBACK_NEUTRAL - robot cycles between forward and reverse translation for net zero translation
    //If motor 2 (or motor 1) is not present - control sequence remains identical

    //handle translating forwards
    if (melty_parameters.translate_forback == RC_FORBACK_FORWARD || (melty_parameters.translate_forback == RC_FORBACK_NEUTRAL && cycle_count % 2 == 0)) {
      if (time_spent_this_rotation_us >= melty_parameters.motor_start1 && time_spent_this_rotation_us <= melty_parameters.motor_stop1) {
        motor_1_on();
      } else {
        motor_1_off();
      }
      if (time_spent_this_rotation_us >= melty_parameters.motor_start2 || time_spent_this_rotation_us <= melty_parameters.motor_stop2) {
        motor_2_on();
      } else {
        motor_2_off();
      }
    }

    //handle translating backwards
    if (melty_parameters.translate_forback == RC_FORBACK_BACKWARD || (melty_parameters.translate_forback == RC_FORBACK_NEUTRAL && cycle_count % 2 == 1)) {
      if (time_spent_this_rotation_us >= melty_parameters.motor_start2 || time_spent_this_rotation_us <= melty_parameters.motor_stop2) {
        motor_1_on();
      } else {
        motor_1_off();
      }
      if (time_spent_this_rotation_us >= melty_parameters.motor_start1 && time_spent_this_rotation_us <= melty_parameters.motor_stop1) {
        motor_2_on();
      } else {
        motor_2_off();
      }
    }

    //displays heading LED at correct location
    if (melty_parameters.led_start > melty_parameters.led_stop) {
      if (time_spent_this_rotation_us >= melty_parameters.led_start || time_spent_this_rotation_us <= melty_parameters.led_stop) {
        heading_led_on();
      } else {
        heading_led_off();
      }
    } else {
      if (time_spent_this_rotation_us >= melty_parameters.led_start && time_spent_this_rotation_us <= melty_parameters.led_stop) {
        heading_led_on();
      } else {
        heading_led_off();
      }
    }

    //just updating at end of loop to assure same value used for all evaluations
    time_spent_this_rotation_us = micros() - start_time;
  }
}