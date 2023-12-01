#include "melty_config.h"

#ifdef ENABLE_EEPROM_STORAGE

float load_accel_mount_radius();

float load_accel_zero_g_offset();

int load_heading_led_offset();

void save_settings_to_eeprom(int led_offset, float accel_radius, float accel_zero_g_offset);

#endif