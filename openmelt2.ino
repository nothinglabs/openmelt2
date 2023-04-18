#include "rc_handler.h"
#include "melty_config.h"
#include "motor_driver.h"
#include "accel_handler.h"
#include "spin_control.h"
#include "config_storage.h"

//#define JUST_DO_DIAGNOSTIC_LOOP

void setup() {

  init_rc();
  init_accel();
  init_motors();
  init_melty();

  Serial.begin(115200);

  //do a little pulse at boot-up to make it clear we booted
  for (int x = 0; x < 30; x++) {
    heading_led_on(0);
    delay(10);
    heading_led_off();
    delay(20);
  }

//load settings on boot
 #ifdef ENABLE_EEPROM_STORAGE  
  load_melty_config_settings();
#endif

}

//dumps out diagnostics info
void diagnostic_loop() {

  float battery_voltage = (analogRead(BATTERY_ADC_PIN) * (ARDUINIO_VOLTAGE / 1023.0)) * VOLTAGE_DIVIDER;

  Serial.print(battery_voltage);
  Serial.print(", ");
  
#ifdef ENABLE_EEPROM_STORAGE  
  Serial.print(load_accel_mount_radius());
  Serial.print(", ");
  Serial.print(load_heading_led_offset());
  Serial.print(", ");
#endif  

  Serial.print(rc_signal_is_healthy());
  Serial.print(", ");
  Serial.print(get_accel_force_g());
  Serial.print(", ");
  Serial.print(rc_get_throttle_percent());
  Serial.print(", ");
  Serial.print(rc_get_leftright());
  Serial.print(", ");
  Serial.print(rc_get_forback());
  Serial.print(", ");
  Serial.println(get_max_rpm());
}

void loop() {

#ifdef JUST_DO_DIAGNOSTIC_LOOP
  while (1) {
    diagnostic_loop();
  }
#endif

//if the rc signal isn't good - keep motors off - and cycle slow LED pulse
  while (rc_signal_is_healthy() != RC_SIGNAL_GOOD) {
    motors_off();
    heading_led_on(0);
    delay(50);
    heading_led_off();
    delay(300);    
  }

  //if RC is good - and throtte is above 0 - spin a single rotation (repeat as needed...)
  if (rc_get_throttle_percent() > 0) {
  
    //this is where all the motor control happens!  (see spin_control.cpp)
    spin_one_rotation();  
  
  } else {
    
    //otherwise - assure motors are off
    motors_off();

    //"fast" idle flash - indicates RC signal is good while sitting idle
    delay(50);
    heading_led_on(0);
    // adjusts flashing pattern to let user know bot is in config mode
    if (get_config_mode() == 1) delay(100);
    delay(50);
    heading_led_off();    
    
    
    //if user pushes control stick up / holds for 750ms - flashes out top speed (100's of RPMs)
    if (rc_get_forback() == RC_FORBACK_FORWARD) {
      delay(750);
      if (rc_get_forback() == RC_FORBACK_FORWARD) {
        for (int x = 0; x < get_max_rpm(); x = x + 100) {
          delay(600);
          heading_led_on(0);
          delay(20);
          heading_led_off();
        }
        delay(1200);
      }
    }

    //if user pulls control stick back for 750ms - enters interactive configuration mode
    if (rc_get_forback() == RC_FORBACK_BACKWARD) {
      delay(750);
      if (rc_get_forback() == RC_FORBACK_BACKWARD) {
        toggle_config_mode(); 
        if (get_config_mode() == 0) save_melty_config_settings();    //save melty settings on config mode exit
      }
    }    
  }
}