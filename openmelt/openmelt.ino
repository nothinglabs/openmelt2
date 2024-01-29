//See melty_config.h for configuration parameters

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

//loops until a good RC signal is detected and throttle is zero (assures safe start)
static void wait_for_rc_good_and_zero_throttle() {
    while (rc_signal_is_healthy() == false || rc_get_throttle_percent() > 0) {
      
      //"slow on/off" for LED while waiting for signal
      heading_led_on(0); delay(250);
      heading_led_off(); delay(250);
      
      //services watchdog and echo diagnostics while we are waiting for RC signal
      service_watchdog();
      echo_diagnostics();
  }
}
  

//Arduino initial setup function
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

//if JUST_DO_DIAGNOSTIC_LOOP - then we just loop and display debug info via USB (good for testing)
#ifdef JUST_DO_DIAGNOSTIC_LOOP
  while (1) {
    service_watchdog();
    echo_diagnostics();
    delay(250);   //delay prevents serial from getting flooded (can cause issues programming)
  }
#endif

#ifdef VERIFY_RC_THROTTLE_ZERO_AT_BOOT 
  wait_for_rc_good_and_zero_throttle();     //Wait for good RC signal at zero throttle
  delay(MAX_MS_BETWEEN_RC_UPDATES + 1);     //Wait for first RC signal to have expired
  wait_for_rc_good_and_zero_throttle();     //Verify RC signal is still good / zero throttle
#endif

}

//dumps out diagnostics info
static void echo_diagnostics() {

  Serial.print("Raw Accel G: "); Serial.print(get_accel_force_g());
  Serial.print("  RC Health: "); Serial.print(rc_signal_is_healthy());
  Serial.print("  RC Throttle: "); Serial.print(rc_get_throttle_percent());
  Serial.print("  RC L/R: "); Serial.print(rc_get_leftright());
  Serial.print("  RC F/B: "); Serial.print(rc_get_forback());

#ifdef BATTERY_ALERT_ENABLED
  Serial.print("  Battery Voltage: "); Serial.print(get_battery_voltage());
#endif 
  
#ifdef ENABLE_EEPROM_STORAGE  
  Serial.print("  Accel Radius: "); Serial.print(load_accel_mount_radius());
  Serial.print("  Heading Offset: "); Serial.print(load_heading_led_offset());
  Serial.print("  Zero G Offset: "); Serial.print(load_accel_zero_g_offset());
#endif  
  Serial.println("");

}

//Used to flash out max recorded RPM 100's of RPMs
static void display_rpm_if_requested() {
  //triggered by user pushing throttle up while bot is at idle for 750ms
  if (rc_get_forback() == RC_FORBACK_FORWARD) {
    delay(750);
     //verify throttle at zero to prevent accidental entry into RPM flash
    if (rc_get_forback() == RC_FORBACK_FORWARD && rc_get_throttle_percent() == 0) {
       
      //throttle up cancels RPM count
      for (int x = 0; x < get_max_rpm() && rc_get_throttle_percent() == 0; x = x + 100) {
        service_watchdog();   //flashing out RPM can take a while - need to assure watchdog doesn't trigger
        delay(600); heading_led_on(0);
        delay(20); heading_led_off();
      }
      delay(1500);  //flash-out punctuated with delay to make clear RPM count has completed
    }
  }
}

//checks if user has requested to enter / exit config mode
static void check_config_mode() {
  //if user pulls control stick back for 750ms - enters (or exits) interactive configuration mode
  if (rc_get_forback() == RC_FORBACK_BACKWARD) {
    delay(750);
    if (rc_get_forback() == RC_FORBACK_BACKWARD) {
      toggle_config_mode(); 
      if (get_config_mode() == false) save_melty_config_settings();    //save melty settings on config mode exit
      
      //wait for user to release stick - so we don't re-toggle modes
      while (rc_get_forback() == RC_FORBACK_BACKWARD) {
        service_watchdog();
      }
    }
  }    
}

//handles the bot when not spinning (with RC good)
static void handle_bot_idle() {

    motors_off();               //assure motors are off
    
    //normal LED "fast flash" - indicates RC signal is good while sitting idle
    heading_led_on(0); delay(30);
    heading_led_off(); delay(120);

    //if in config mode blip LED again to show "double-flash" 
    if (get_config_mode() == true) {
      heading_led_off(); delay(400);
      heading_led_on(0); delay(30);
      heading_led_off(); delay(140);
    }

    check_config_mode();          //check if user requests we enter / exit config mode
    display_rpm_if_requested();   //flashed out RPM if user has requested

    echo_diagnostics();           //echo diagnostics if bot is idle
}

//main control loop
void loop() {

  service_watchdog();             //keep the watchdog happy

  //if the rc signal isn't good - assure motors off - and "slow flash" LED
  //this will interrupt a spun-up bot if the signal becomes bad
  while (rc_signal_is_healthy() == false) {
    motors_off();
    
    heading_led_on(0); delay(30);
    heading_led_off(); delay(600);
    
    //services watchdog and echo diagnostics while we are waiting for RC signal
    service_watchdog();
    echo_diagnostics();
  }

  //if RC is good - and throtte is above 0 - spin a single rotation
  if (rc_get_throttle_percent() > 0) {
    //this is where all the motor control happens!  (see spin_control.cpp)
    spin_one_rotation();  
  } else {    
    handle_bot_idle();
  }

}