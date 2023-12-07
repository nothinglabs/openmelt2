#include "rc_handler.h"
#include "melty_config.h"
#include "motor_driver.h"
#include "accel_handler.h"
#include "spin_control.h"
#include "config_storage.h"
#include "led_driver.h"
#include "battery_monitor.h"


#ifdef ENABLE_WATCHDOG
#include <Adafruit_SleepyDog.h>
#endif

void service_watchdog() {
#ifdef ENABLE_WATCHDOG
    Watchdog.reset();
#endif
}


void wait_for_rc_good_and_zero_throttle() {
    while (rc_signal_is_healthy() != RC_SIGNAL_GOOD || rc_get_throttle_percent() > 0) {
      service_watchdog();
      heading_led_on(0);
      delay(250);
      heading_led_off();
      delay(250);
  }
}
  

void setup() {
  
  Serial.begin(115200);

  //get motor drivers setup (and off!) first thing
  init_motors();
  init_led();


#ifdef ENABLE_WATCHDOG
    //returns actual watchdog timeout MS
    int watchdog_ms = Watchdog.enable(WATCH_DOG_TIMEOUT_MS);
#endif

  init_rc();
  init_accel();   //accelerometer uses i2c - which can fail blocking (so only initializing it -after- the watchdog is running)
  
//load settings on boot
#ifdef ENABLE_EEPROM_STORAGE  
  load_melty_config_settings();
#endif

#ifdef JUST_DO_DIAGNOSTIC_LOOP
  while (1) {
    service_watchdog();
    diagnostic_loop();
    delay(250);   //delay prevents serial from getting flooded (can cause issues programming)
  }
#endif

//1. Wait for good RC signal at zero throttle
//2. Wait for first RC signal to have expired
//3. Verify RC signal is still good / zero throttle

#ifdef VERIFY_RC_THROTTLE_ZERO_AT_BOOT 
  wait_for_rc_good_and_zero_throttle();
  delay(MAX_MS_BETWEEN_RC_UPDATES + 1);
  wait_for_rc_good_and_zero_throttle();
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
  Serial.print(get_accel_force_g()  - load_accel_zero_g_offset());

  Serial.print("  Battery V: ");
  Serial.print(get_battery_voltage());


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
     //prevent accidental entry into RPM flash / throttle up cancels RPM count
    if (rc_get_forback() == RC_FORBACK_FORWARD && rc_get_throttle_percent() == 0) {
      for (int x = 0; x < get_max_rpm() && rc_get_throttle_percent() == 0; x = x + 100) {
        service_watchdog();   //flashing out RPM can take a while - need to assure watchdog doesn't trigger
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

  service_watchdog();

//if the rc signal isn't good - keep motors off - and cycle slow LED pulse
  while (rc_signal_is_healthy() != RC_SIGNAL_GOOD) {
    service_watchdog();
    motors_off();
    heading_led_on(0);
    delay(30);
    heading_led_off();
    delay(600);
  }

  //if RC is good - and throtte is above 0 - spin a single rotation (repeat as needed...)
  if (rc_get_throttle_percent() > 0) {
     //this is where all the motor control happens!  (see spin_control.cpp)
    spin_one_rotation();  
  
  } else {
    
    //otherwise - assure motors are off
    motors_off();
    
    //"fast" idle flash - indicates RC signal is good while sitting idle
    delay(150);
    heading_led_on(0);
    delay(30);
    heading_led_off();

    //extra delay adjusts flashing pattern to let user know bot is in config mode
    if (get_config_mode() == 1) {
      delay(75);
      heading_led_on(0);
      delay(70);
      heading_led_off();
      delay(70);
    }

    check_config_mode();
    display_rpm_if_requested();
  }

}