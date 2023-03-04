
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

//This value determines how much movement of the RC stick effects heading adjustment
//Lesser values cause greater change (greater values cause lesser change)
#define LEFT_RIGHT_HEADING_CONTROL_DIVISOR 2

#define ACCEL_MOUNT_RADIUS_CM 4.0
#define LED_OFFSET_PERCENT 44   //adjust to make heading LED line up with direction robot travels

//full power spin in below this number
//TODO: These number should be much faster (just testing)
#define MIN_TRANSLATION_RPM                    250
#define MAX_TRANSLATION_ROTATION_INTERVAL_US   (1.0f / MIN_TRANSLATION_RPM) * 60 * 1000 * 1000

//don't even try to do heading track if we are this slow
//limits max time spent in do_melty (helps assure heartbeat is checked at safe interval)
#define MAX_TRACKING_ROTATION_INTERVAL_US   MAX_TRANSLATION_ROTATION_INTERVAL_US * 2

