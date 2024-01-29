#include "melty_config.h"

#ifdef ENABLE_EEPROM_STORAGE

//retrieves accelerometer mount radius from EEPROM
float load_accel_mount_radius();

//retrieves zero G offset from EEPROM
float load_accel_zero_g_offset();

//retrieves LED offset from EEPROM
int load_heading_led_offset();

//saves all settings to EEPROM
void save_settings_to_eeprom(int led_offset, float accel_radius, float accel_zero_g_offset);

#endif