//check for low battery - but only alarm after certain number of low reads in a row (prevents ADC noise from alarming)
//(settings in melty_config.h)
bool battery_voltage_low();

//returns battery voltage
float get_battery_voltage();