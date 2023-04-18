void init_melty(void);

void spin_one_rotation(void);

void heading_led_on(int led_shimmering);
 
void heading_led_off();

int get_max_rpm();

void toggle_config_mode();

int get_config_mode();

void load_melty_config_settings();

void save_melty_config_settings();

//holds melty parameters used to determine timing for current spin cycle
//all time offsets are in microseconds
typedef struct melty_parameters_t {
  int translate_forback;  //RC_FORBACK_FORWARD, RC_FORBACK_BACKWARD, RC_FORBACK_NETURAL
	unsigned long rotation_interval_us; //time for 1 rotation of robot
	unsigned long led_start;  //offset for beginning of LED beacon
	unsigned long led_stop;   //offset for end of LED beacon
	unsigned long motor_start1; //time offset for when motor 1 turns on
	unsigned long motor_stop1;  //time offset for when motor 1 stops
	unsigned long motor_start2; //"" for motor 2 (times can overlap with motor 1)
	unsigned long motor_stop2;  //""
};
