//All pulse lengths in microseconds
//it's accepted that a TX with fully centered trims
//may produce values somewhat off these numbers

//RC pulses outside this range are considered invalid (indicate a bad RC signal)
#define MAX_RC_PULSE_LENGTH 2400
#define MIN_RC_PULSE_LENGTH 700

//This value reflects nominal range of possible RC pulse values (maximum - minimum)
//This value is used to help scale left / right adjustment of heading
//(does not need to be perfect)
#define NOMINAL_PULSE_RANGE (MAX_RC_PULSE_LENGTH - MIN_RC_PULSE_LENGTH)

//pulses below this value are considered 0% throttle
#define IDLE_THROTTLE_PULSE_LENGTH 1250
//pulses above this value are considered 100%
#define FULL_THROTTLE_PULSE_LENGTH 1850

//center value for left / right
#define CENTER_LEFTRIGHT_PULSE_LENGTH 1500

//center value for for / back
#define CENTER_FORBACK_PULSE_LENGTH 1500

//if pulse length must differ by at least this much
//from CENTER_FORBACK_PULSE_LENGTH to be considered
//going forward or back
#define FORBACK_MIN_THRESH_PULSE_LENGTH 100

//deadzone for LR when in config mode (in US)
//(for actual translation control - there is no deadzone to allow for trim adjustment)
#define LR_CONFIG_MODE_DEADZONE_WIDTH 100

#define RC_FORBACK_FORWARD 1
#define RC_FORBACK_NEUTRAL 0
#define RC_FORBACK_BACKWARD -1

#define RC_SIGNAL_GOOD 1
#define RC_SIGNAL_BAD 0

#define RC_LR_IN_CONFIG_DEADZONE 1
#define RC_LR_NOT_IN_CONFIG_DEADZONE 0

//if we don't get a valid RC update on the throttle at least this often - spin down
#define MAX_US_BETWEEN_RC_UPDATES 200000

void init_rc();
int rc_signal_is_healthy();
int rc_get_throttle_percent();
int rc_get_forback();
int rc_get_leftright();

int rc_get_is_lr_in_config_deadzone();

void lock_rc_data();
void unlock_rc_data();