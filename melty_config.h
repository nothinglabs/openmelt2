
//NOTE: This code doesn't currently support persistent config storage for ARM-based Arduinos
//(see config_storage.cpp for details)
#define ENABLE_EEPROM_STORAGE   //Comment out this to disable EEPROM (for ARM)

//must be Arduino interrupt pins
//we need 3 interrupt pins - which requires an Arduino with Atmega32u4 or better
//(Atmega328 only support 2 interrupts)
#define FORBACK_RC_CHANNEL_PIN 1
#define LEFTRIGHT_RC_CHANNEL_PIN 7
#define THROTTLE_RC_CHANNEL_PIN 0

#define HEADING_LED_PIN			8

//no configuration changes are needed if only 1 motor is used!
#define MOTOR_PIN1				9
#define MOTOR_PIN2				10

//default values - can override with interactive config
#define DEFAULT_ACCEL_MOUNT_RADIUS_CM 5.9
#define DEFAULT_LED_OFFSET_PERCENT 60   //adjust to make heading LED line up with direction robot travels (increasing moves beacon clockwise)

//This value determines how much movement of the RC stick effects heading adjustment
//Lesser values cause greater change (greater values cause lesser change - 1.0f seems to work well)
#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 1.0f

//Determines how quickly the effective radius is adjusted when in config mode (larger values = slower)
#define LEFT_RIGHT_CONFIG_RADIUS_ADJUST_DIVISOR 50.0f

//Determines how quickly the LED heading is adjusted when in config mode (larger values = slower)
#define LEFT_RIGHT_CONFIG_LED_ADJUST_DIVISOR 0.4f

//full power spin in below this number
#define MIN_TRANSLATION_RPM                    300
#define MAX_TRANSLATION_ROTATION_INTERVAL_US   (1.0f / MIN_TRANSLATION_RPM) * 60 * 1000 * 1000

//don't even try to do heading track if we are this slow
//limits max time spent in do_melty (helps assure heartbeat is checked at safe interval)
#define MAX_TRACKING_ROTATION_INTERVAL_US   MAX_TRANSLATION_ROTATION_INTERVAL_US * 2