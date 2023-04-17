#include "melty_config.h"

#ifdef ENABLE_EEPROM_STORAGE

float load_accel_mount_radius();

int load_heading_led_offset();

void save_settings_to_eeprom(int led_offset, float accel_radius);

#endif