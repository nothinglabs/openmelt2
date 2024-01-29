
//does translational drift rotation (robot spins 360 degrees)
void spin_one_rotation(void);

//returns maximum rotation speed since last entering config mode
int get_max_rpm();

//toggles configuration mode
void toggle_config_mode();

//returns true if in configuration mode
bool get_config_mode();

//triggers load of melty parameters
void load_melty_config_settings();

//saves melty parameters
void save_melty_config_settings();

//holds melty parameters used to determine timing for current spin cycle
//all time offsets are in microseconds

typedef struct melty_parameters_t {
  int translate_forback;              //RC_FORBACK_FORWARD, RC_FORBACK_BACKWARD, RC_FORBACK_NETURAL
  float throttle_percent;             //stores throttle percent
	unsigned long rotation_interval_us; //time for 1 rotation of robot
	unsigned long led_start;            //offset for beginning of LED beacon
	unsigned long led_stop;             //offset for end of LED beacon
	unsigned long motor_start_phase_1;  //time offset for when motor 1 turns on when translating forward (or motor 2 translating backwards)
	unsigned long motor_stop_phase_1;   //time offset for when motor 1 turns off when translating forward (or motor 2 translating backwards)
	unsigned long motor_start_phase_2;  //time offset for when motor 2 turns on when translating forward (or motor 1 translating backwards)
	unsigned long motor_stop_phase_2;   //time offset for when motor 2 turns on when translating forward (or motor 1 translating backwards)
  int steering_disabled;              //Prevents adjustment of left / right heading adjustment (used for configuration mode)
  int led_shimmer;                    //LED is shimmering to indicate something to the user
};
