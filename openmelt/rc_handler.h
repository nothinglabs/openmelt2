
//used to return forward / back control stick position
typedef enum {
    RC_FORBACK_FORWARD = 1,     //control stick pushed forward
    RC_FORBACK_NEUTRAL = 0,     //control stick neutral
    RC_FORBACK_BACKWARD = -1     //control stick held back
} rc_forback;

void init_rc();

bool rc_signal_is_healthy();           //return true if RC signal looks good

int rc_get_throttle_percent();        //returns 0-100 value indicating throttle level

rc_forback rc_get_forback();          //returns RC_FORBACK_FORWARD, RC_FORBACK_NEUTRAL or RC_FORBACK_BACKWARD depending on stick position
int rc_get_leftright();               //returns offset in microseconds from center value (not converted to percentage)

//these functions return true if L/R stick movement is below defined thresholds
bool rc_get_is_lr_in_config_deadzone();  
bool rc_get_is_lr_in_normal_deadzone();

//All pulse lengths in microseconds
//it's accepted that a TX with fully centered trims may produce values somewhat off these numbers

//RC pulses outside this range are considered invalid (indicate a bad RC signal)
#define MAX_RC_PULSE_LENGTH 2400
#define MIN_RC_PULSE_LENGTH 700

//This value reflects nominal range of possible RC pulse values (maximum - minimum)
//This value is used to help scale left / right adjustment of heading
//(does not need to be perfect)
#define NOMINAL_PULSE_RANGE (MAX_RC_PULSE_LENGTH - MIN_RC_PULSE_LENGTH)

#define IDLE_THROTTLE_PULSE_LENGTH 1250           //pulses below this value are considered 0% throttle
#define FULL_THROTTLE_PULSE_LENGTH 1850           //pulses above this value are considered 100%
#define CENTER_LEFTRIGHT_PULSE_LENGTH 1500        //center value for left / right
#define CENTER_FORBACK_PULSE_LENGTH 1500          //center value for for / back

#define FORBACK_MIN_THRESH_PULSE_LENGTH 100       //pulse length must differ by this much from CENTER_FORBACK_PULSE_LENGTH to be considered going forward or back

#define LR_CONFIG_MODE_DEADZONE_WIDTH 100         //deadzone for LR when in config mode (in US) - prevents unintended tracking adjustments
#define LR_NORMAL_DEADZONE_WIDTH 25               //deadzone for normal drive - can help with unintentional drift when moving forward / back

#define MAX_MS_BETWEEN_RC_UPDATES 900             //if we don't get a valid RC update on the throttle at least this often - spin down



