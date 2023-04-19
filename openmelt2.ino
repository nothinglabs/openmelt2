#include "rc_handler.h"
#include "melty_config.h"
#include "motor_driver.h"
#include "accel_handler.h"
#include "spin_control.h"
#include "config_storage.h"
#include "led_driver.h"
#include "battery_monitor.h"

void setup() {
  
  Serial.begin(115200);

  init_rc();
  init_accel();
  init_motors();
  init_led();

  //do a little pulse at boot-up to make it clear we booted
  
  for (int x = 0; x < 4; x++) {
    heading_led_on(0);
    delay(300);
    heading_led_off();
    delay(300);
  }
  
//load settings on boot
 #ifdef ENABLE_EEPROM_STORAGE  
  load_melty_config_settings();
#endif

#ifdef JUST_DO_DIAGNOSTIC_LOOP
  while (1) {
    diagnostic_loop();
  }
#endif

}

//dumps out diagnostics info
void diagnostic_loop() {

#ifdef BATTERY_ALERT_ENABLED
  Serial.print("  Battery Voltage: ");
  Serial.print(get_battery_voltage());
#endif 
  
#ifdef ENABLE_EEPROM_STORAGE  
  Serial.print("  Accel Radius: ");
  Serial.print(load_accel_mount_radius());
  Serial.print("  Heading Offset: ");
  Serial.print(load_heading_led_offset());
#endif  

  Serial.print("  Accel G: ");
  Serial.print(get_accel_force_g());

  Serial.print("  RC Health: ");
  Serial.print(rc_signal_is_healthy());
  Serial.print("  RC Throttle: ");
  Serial.print(rc_get_throttle_percent());
  Serial.print("  RC L/R: ");
  Serial.print(rc_get_leftright());
  Serial.print("  RC F/B: ");
  Serial.println(rc_get_forback());
}

void display_rpm_if_requested() {
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
}

void check_config_mode() {
  //if user pulls control stick back for 750ms - enters interactive configuration mode
  if (rc_get_forback() == RC_FORBACK_BACKWARD) {
    delay(750);
    if (rc_get_forback() == RC_FORBACK_BACKWARD) {
      toggle_config_mode(); 
      if (get_config_mode() == 0) save_melty_config_settings();    //save melty settings on config mode exit
    }
  }    
}


void loop() {
  
//if the rc signal isn't good - keep motors off - and cycle slow LED pulse
  while (rc_signal_is_healthy() != RC_SIGNAL_GOOD) {
    motors_off();
    heading_led_on(0);
    delay(50);
    heading_led_off();
    delay(400);    
  }

  //if RC is good - and throtte is above 0 - spin a single rotation (repeat as needed...)
  if (rc_get_throttle_percent() > 0) {
     //this is where all the motor control happens!  (see spin_control.cpp)
    spin_one_rotation();  
  
  } else {
    
    //otherwise - assure motors are off
    motors_off();
    
    //"fast" idle flash - indicates RC signal is good while sitting idle
    delay(20);
    heading_led_on(0);
    delay(40);
    heading_led_off();

    //extra delay adjusts flashing pattern to let user know bot is in config mode
    if (get_config_mode() == 1) {
      delay(200);
      heading_led_on(0);
      delay(20);
      heading_led_off();
    }

    check_config_mode();
    display_rpm_if_requested();
  }

}