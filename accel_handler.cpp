//this module interfaces with the accelerometer to provide current G-force level

//Tested with Sparkfun's H3LIS331DL breakout: https://www.sparkfun.com/products/14480 (3v part)
//Using SparkFun Logic Level Converter https://www.sparkfun.com/products/12009 to interface with 5v arduino

//Adafruit's HSLIS311 breakout includes a level convert - and should be a single component solution:
//https://www.adafruit.com/product/4627

//Sparkfun's LIS311 library is used - which should also work for Adafruit part

#include <arduino.h>
#include "melty_config.h"
#include "accel_handler.h"
#include <Wire.h>
#include "SparkFun_LIS331.h"


LIS331 xl;

void init_accel() {

  Wire.begin();
  
  Wire.setClock(400000);  //increase I2C speed to reduce read times a bit
                          //value of 400000 allows accel read in ~1ms and
                          //is verfied to work with Sparkfun level converter
                          //(some level converters have issues at higher speeds)
  
  xl.setI2CAddr(0x19);
  xl.begin(LIS331::USE_I2C);

  //sets accelerometer to specified scale (100, 200, 400g)
  xl.setFullScale(ACCEL_RANGE);
}

//reads accel and converts to G's
//ACCEL_MAX_SCALE needs to match ACCEL_RANGE value
float get_accel_force_g() {
  int16_t x, y, z;
  xl.readAxes(x, y, z);
  return xl.convertToG(ACCEL_MAX_SCALE,x);
}