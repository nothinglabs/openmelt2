# Open Melt 2

Open Melt is an open-source translational drift (aka "melty brain") robot controller project.  

A translational drift robot constantly spins its entire body using its drive wheel(s), but is still capable of directional control by modulating motor power at certain points each rotation.  To acheive this - the rate of rotation must be tracked.

Open Melt uses an accelerometer to calculate the rate of rotation based on G-forces around a given radius (centrifugal force).  For example - an accelerometer mounted 4cm from the center of rotation experiencing 145g can be [calculated] (https://druckerdiagnostics.com/g-force-calculator/) to indicate 1800rpm rotation.

An LED is turned on once per rotation - giving the appearance of the "front" of the robot.  This allows the driver to see the expected direction of translation.  The user can adjust the heading beacon by moving the remote control left or right.  This also allows for steering and correction of minor tracking errors.

This system can work with robots using either one or two drive motors.

Version 2 of Open Melt is based on Arduino.

Plans for a 3d-printed "antweight" (1lbs) combat robot are provided.


##General Hardware Requirements

- Arduino Micro (or other Atmega32u4 5v 16MHz Arduino)
- [Adafruit HSLIS311 breakout](https://www.adafruit.com/product/4627)
- RC Transmittter / Receiver 
- Heading LED
- Motor Driver(s)
- Motor(s)
- 4700uF capacitor

Adafruit's ItsyBitsy 32u4 - 5V 16MHz has been tested and works.

The HSLI311 is a 3v part - but the Adafruit breakout conveniently includes a 3v<->5v level converter.  Alternatively, [Sparkfun's H3LIS331DL Breakout](https://www.sparkfun.com/products/14480) may be used in conjunction with the [SparkFun 3v<->5v Logic Level Converter](https://www.sparkfun.com/products/12009) (tested).

Use of an Atmega328-based Arduino is **not** supported due to lack of adequate interrupt pins.
 
Using an ARM-based Arduino has not been tested - but may be possible with code modication (see notes in melty_config.h	).

The 4700uF capacitor is required across the 5v power bus to assure motor noise does not cause power fluctuations (and unwanted reboots).

##Schematic

##Reference Platform

##Interactive Configuration
