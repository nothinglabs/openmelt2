//This module handles the RC interface (interrupt driven)

#include "rc_handler.h"
#include "arduino.h"
#include "melty_config.h"

#define RC_DATA_UNLOCKED 0
#define RC_DATA_LOCKED 1 

static int rc_data_lock_state = RC_DATA_UNLOCKED;

//config / current values for each RC channel
struct rc_channel_t {
  int pin;                        //pin channel is connected to
  unsigned long pulse_length;     //most recent pulse length in us
  unsigned long pulse_start_time; //time stamp of when RC pin last went high
  unsigned long last_good_signal; //time stamp (MS) of when last pulse of valid length was received
};

static struct rc_channel_t forback_rc_channel = {
  .pin = FORBACK_RC_CHANNEL_PIN,
  .pulse_length = MIN_RC_PULSE_LENGTH,
  .pulse_start_time = 0,
  .last_good_signal = 0
};

static struct rc_channel_t leftright_rc_channel = {
  .pin = LEFTRIGHT_RC_CHANNEL_PIN,
  .pulse_length = MIN_RC_PULSE_LENGTH,
  .pulse_start_time = 0,
  .last_good_signal = 0
};

static struct rc_channel_t throttle_rc_channel = {
  .pin = THROTTLE_RC_CHANNEL_PIN,
  .pulse_length = MIN_RC_PULSE_LENGTH,
  .pulse_start_time = 0,
  .last_good_signal = 0
};

//prevent RC data from changing when in use
static void lock_rc_data() {
  rc_data_lock_state = RC_DATA_LOCKED;
}

static void unlock_rc_data() {
  rc_data_lock_state = RC_DATA_UNLOCKED;
}

//updates RC channels with latest values
static void update_rc_channel(struct rc_channel_t *rc_channel) {

//if we are using the rc channel info - don't update it!

  if (rc_data_lock_state == RC_DATA_LOCKED) return;
  int rc_channel_current_state = digitalRead(rc_channel->pin);

  //pulse started
  if (rc_channel_current_state == 1) {
    rc_channel->pulse_start_time = micros();
  }

  //pulse ended
  if (rc_channel_current_state == 0) {
    //only assign new value if micros() didn't overflow
    //(overflow just causes single missed update)
    if (micros() > rc_channel->pulse_start_time) {
      //protect against missing end of pulse / triggering on next fall
      unsigned long new_pulse_length = micros() - rc_channel->pulse_start_time;
      if (new_pulse_length <= MAX_RC_PULSE_LENGTH && new_pulse_length >= MIN_RC_PULSE_LENGTH) {
        rc_channel->pulse_length = new_pulse_length;
        rc_channel->last_good_signal = millis();
      }
    }
  }
}

//verifies RC channels have returned values more
//recent that MAX_MS_BETWEEN_RC_UPDATES
bool rc_signal_is_healthy() {
  
  lock_rc_data();
  unsigned long last_good_signal = throttle_rc_channel.last_good_signal;
  unlock_rc_data();
  
  //initial signal not received
  if (last_good_signal == 0) return false;

  if (millis() - last_good_signal > MAX_MS_BETWEEN_RC_UPDATES) return false;
  
  return true;
}

//returns at integer from 0 to 100 based on throttle position
//default values are intended to have "dead zones" at both top
//and bottom of stick for 0 and 100 percent
int rc_get_throttle_percent() {

  lock_rc_data();
  unsigned long pulse_length = throttle_rc_channel.pulse_length;
  unlock_rc_data();

  if (pulse_length >= FULL_THROTTLE_PULSE_LENGTH) return 100;
  if (pulse_length <= IDLE_THROTTLE_PULSE_LENGTH) return 0;

  long throttle_percent = (pulse_length - IDLE_THROTTLE_PULSE_LENGTH) * 100;
  throttle_percent = throttle_percent / (FULL_THROTTLE_PULSE_LENGTH - IDLE_THROTTLE_PULSE_LENGTH);
  return (int)throttle_percent;
}

bool rc_get_is_lr_in_config_deadzone() {
  if (abs(rc_get_leftright()) < LR_CONFIG_MODE_DEADZONE_WIDTH) return true;
  return false;
}

bool rc_get_is_lr_in_normal_deadzone() {
  if (abs(rc_get_leftright()) < LR_NORMAL_DEADZONE_WIDTH) return true;
  return false;
}


//returns RC_FORBACK_FORWARD, RC_FORBACK_BACKWARD or RC_FORBACK_NEUTRAL based on stick position
rc_forback rc_get_forback() {
  
  lock_rc_data();
  unsigned long pulse_length = forback_rc_channel.pulse_length;
  unlock_rc_data();

  int rc_forback_offset = pulse_length - CENTER_FORBACK_PULSE_LENGTH;
  if (rc_forback_offset > FORBACK_MIN_THRESH_PULSE_LENGTH) return RC_FORBACK_FORWARD;
  if (rc_forback_offset < (FORBACK_MIN_THRESH_PULSE_LENGTH * -1)) return RC_FORBACK_BACKWARD;
  return RC_FORBACK_NEUTRAL;
}

//returns offset in microseconds from center value (not converted to percentage)
//0 for hypothetical perfect center (reality is probably +/-50)
//returns negative value for left / positive value for right
int rc_get_leftright() {
  
  lock_rc_data();
  unsigned long pulse_length = leftright_rc_channel.pulse_length;
  unlock_rc_data();

  return pulse_length - CENTER_LEFTRIGHT_PULSE_LENGTH;
}

//ISRs for each RC interrupt pin
void forback_rc_change() {
  update_rc_channel(&forback_rc_channel);
}
void leftright_rc_change() {
  update_rc_channel(&leftright_rc_channel);
}
void throttle_rc_change() {
  update_rc_channel(&throttle_rc_channel);
}

//attach interrupts to rc pins
void init_rc(void) {
  attachInterrupt(digitalPinToInterrupt(forback_rc_channel.pin), forback_rc_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftright_rc_channel.pin), leftright_rc_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(throttle_rc_channel.pin), throttle_rc_change, CHANGE);
}

