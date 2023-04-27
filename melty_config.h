//NOTE: This code doesn't currently support persistent config storage for ARM-based Arduinos
//(see config_storage.cpp for details)

//#define JUST_DO_DIAGNOSTIC_LOOP                 //Disables the robot / displays config / battery voltage / RC info via serial

#define ENABLE_EEPROM_STORAGE                     //Comment out this to disable EEPROM (for ARM)
#define EEPROM_WRITTEN_SENTINEL_VALUE 43          //Changing this value will cause existing EEPROM values to be invalidated (revert to defaults)

//These values can be overriden with interactive config (loaded from EEPROM on boot)
#define DEFAULT_ACCEL_MOUNT_RADIUS_CM 5.9         //Radius of accelerometer from center of robot
#define DEFAULT_LED_OFFSET_PERCENT 60             //Adjust to make heading LED line up with direction robot travels (increasing moves beacon clockwise)

#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 1.0f   //How quick steering is (larger values = slower)

#define MIN_TRANSLATION_RPM 300                   //full power spin in below this number (increasing can reduce spin-up time)

//RC pins must be Arduino interrupt pins
//we need 3 interrupt pins - which requires an Arduino with Atmega32u4 or better (Atmega328 only support 2 interrupts)
#define FORBACK_RC_CHANNEL_PIN 1                  //To Forward / Back on RX
#define LEFTRIGHT_RC_CHANNEL_PIN 7                //To Left / Right on RX
#define THROTTLE_RC_CHANNEL_PIN 0                 //To Throttle on RX

#define HEADING_LED_PIN	8                         //To heading LED

//no configuration changes are needed if only 1 motor is used!
#define MOTOR_PIN1 9                              //Pin for Motor 1 driver
#define MOTOR_PIN2 10                             //Pin for Motor 2 driver

//High-speed Brushless ESC support
//Only works with high speed PWM (490hz) supporting ESCs (tested with Hobbypower 30A / "Simonk" firmware)
//Assumes Arduino PWM output is 490hz (such as Arduino Micro pins 9 and 10) - should be expected NOT to work with non-AVR Arduino's without changes
//#define ENABLE_HIGH_SPEED_PWM_BRUSHLESS_ESC       //enable high speed (490hz) brushless ESC control
#define PWM_ESC_MOTOR_ON 180                      //Motor PWM ON duty cycle (130 seems barely on / 230 seems a good near-full-throttle value) - setting to 180 for testing
#define PWM_ESC_MOTOR_COAST 130                   //Motor PWM COAST duty cycle - set to same as PWM_ESC_MOTOR_OFF for fully unpowere
                                                  //Setting to provide some power during coast might allow higher RPM / efficiency (at possible cost to translation speed)
#define PWM_ESC_MOTOR_OFF 100                     //Motor PWM OFF duty cycle (100 workd well in testing - if this is too low - ESC may not init)

//Battery monitor constants
#define BATTERY_ALERT_ENABLED                     //if enabled - heading LED will flicker when battery voltage is low
#define BATTERY_ADC_PIN A0                        //Pin for ADC voltage divider 
#define VOLTAGE_DIVIDER 11                        //(~10:1 works well - 10kohm to GND, 100kohm to Bat+).  Resistors have tolerances!  Adjust as needed...
#define BATTERY_ADC_WARN_VOLTAGE_THRESHOLD 10.0f  //If voltage drops below this value - then alert is triggered
#define ARDUINIO_VOLTAGE 5.0f                     //Needed for ADC maths for battery monitor
#define LOW_BAT_REPEAT_READS_BEFORE_ALARM 10      //Requires this many ADC reads below threshold before alarming




