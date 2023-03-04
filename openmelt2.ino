#include "rc_handler.h"
#include "melty_config.h"
#include "motor_driver.h"
#include "accel_handler.h"
#include "spin_control.h"

//#define JUST_DO_DIAGNOSTIC_LOOP

void setup() {
  
  init_rc();
  init_accel();
  init_motors();
  init_melty();

  Serial.begin(115200);

}
void diagnostic_loop() {
    Serial.print(rc_signal_is_healthy());
    Serial.print(", ");
    Serial.print(get_accel_force_g());
    Serial.print(", ");
    Serial.print(rc_get_throttle_percent());
    Serial.print(", ");
    Serial.print(rc_get_leftright());
    Serial.print(", ");
    Serial.println(rc_get_forback());
}

void loop() {

#ifdef JUST_DO_DIAGNOSTIC_LOOP
  while(1) {
    diagnostic_loop();
  }
#endif


  //if RC is good - and throtte is above 0 - spin up
  if (rc_signal_is_healthy() == RC_SIGNAL_GOOD && rc_get_throttle_percent() > 0) {
    spin_one_rotation();
  } else {
  //otherwise - assure motors are off
    motors_off();
    Serial.println("Idle");
      delay(50);
      heading_led_on();
      delay(50);
      heading_led_off();
      diagnostic_loop();

  }


}
