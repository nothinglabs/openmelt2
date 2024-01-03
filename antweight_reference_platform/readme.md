# Open Melt Antweight Reference Platform

This robot was created as an easy-to-build, 3d-printed test platform for Open Melt.  All design files are provided.

Being Arduino based - no special programmer is required.  No custom PCB's or fine-pitch soldering is needed.  All components are readily available as of this writing.

It is intended to comply with [Plastic Antweight combat robot rules](https://www.westernalliedrobotics.com/index.php/the-rules/special-plastic-class-rules).  The total weight of the test build is about 0.5oz under the 16oz weight limit.

This robot can spin up to around 3000rpm.  It is capable of causing projectiles that could seriously injure someone.  It is recommended you only test inside an enclosed arena.

The option for a removable weapon is intended to provide a less-scary way to test a melty brain robot.  Proper precautions should still be taken.

This robot has not yet been battle tested.

<div align="center">
<img src="../media/plastic_ant.jpg" alt="drawing" width="500"/>
</div>

## Parts List
- [Arduino Micro](https://store.arduino.cc/products/arduino-micro) or [Adafruit ItsyBitsy 32u4 - 5V 16MHz](https://www.adafruit.com/product/3677)
- [Adafruit H3LIS331 breakout](https://www.adafruit.com/product/4627)
- RFP30N06LE N-channel MOSFET (widely available)
- [30SQ045 Schottky Diode](https://www.digikey.com/en/products/detail/anbon-semiconductor-int-l-limited/30SQ045/18800935)
- 3mm blue LED (rated 20ma or higher)
- [FlySky i6 transmitter ](https://www.flysky-cn.com/i6-gaishu) and [FlySky iA6 receiver](https://www.flysky-cn.com/ia6-canshu)
- 45-turn 540 size RC car motor
- [FingerTech Mini Power Switch](https://www.fingertechrobotics.com/proddetail.php?prod=ft-mini-switch)
- [FingerTech Twist Hub (3mm / 0.5")](https://www.fingertechrobotics.com/proddetail.php?prod=ft-twist-hubs)
- [FingerTech Foam Wheel (1.75" x 0.5")](https://www.fingertechrobotics.com/proddetail.php?prod=ft-foam-wheels)
- ~900ma 7.4v lithium polymer battery
- 4700uf electrolytic capacitor (30mm or shorter for easy fit)
- 100, 10k and 100k Ohm resistors

## Fasteners / Tools / Misc.:
 - [\#2-28 x 1/4" Phillips Screws](https://www.mcmaster.com/products/plastite-screws/screw-size~no-2/length~1-4-2/length~0-25/)
 - Zip ties
 - Mounting tape (Scotch Extreme or similar)
 - Hot glue gun ("hot" melt as opposed to "low" melt)
 - Small piece of PCB blank or copper sheet
 - Access to a 3d-printer / PLA filament
 - [Wire wrapping tool](https://www.amazon.com/Wire-Wrap-Gauge-ELECTRONIX-EXPRESS/dp/B00BFYE0CY/)

## Overview / Weight Budget

<div align="center">
<img src="../media/internals_labelled.jpg" alt="drawing" width ="750"/>
</div>

| Component                    | Weight (grams) |
|------------------------------|----------------|
| Shell                        | 140            |
| Lid                          | 18             |
| Motor                        | 172            |
| Battery                      | 46             |
| MCU                          | 3              |
| RC Receiver                  | 2              |
| Mosfet + heatsink            | 6              |
| Wires, LED, diode, switch    | 4              |
| Wheel + hub                  | 9              |
| Weapon + pin                 | 30             |
| Power switch                 | 4              |
| 4700uf capacitor             | 4              |
| **Estimated Total**          | **438**        |
| **Measured Total (fully assembled)** | **440 (15.5oz)** |


## 3d Model / OpenSCAD
The body of this robot was designed in [OpenSCAD](https://openscad.org/) (programmatic CAD).  You can easily change parameters to adjust things like shell thickness and diameter.

<table><tr><td>
<img src="../media/shell_with_weapon.png" alt="drawing" width="320"/></td><td>

<img src="../media/shell.png" alt="drawing" width="400"/></td>
</tr><td colspan="2" align="center">
<img src="../media/body_with_electronics.png" alt="drawing" width="350"/></td></tr>
</table>

Stand-ins for electronics can be rendered to verify everything fits as intended.

The following STL files are provided for 3d-printing:

| Component                                              | File                      |
|--------------------------------------------------------|---------------------------|
| Robot shell without weapon (weapon mounting hole)      | shell\_no\_weapon.stl     |
| Detachable weapon                                      | weapon.stl                |
| Weapon mounting pin                                    | weapon\_pin.stl           |
| Robot lid                                              | lid.stl                   |
| Robot shell with integrated weapon                     | shell\_with\_weapon.stl   |



## 3d Printing Notes
The test build of the robot is printed in PLA with 100% infill.

For best strength, use a high shell count (5+) and avoid using a cooling fan.

## General Construction
The lid is held on with \#2 screws.  They seem to self-tap pretty effectively.

The accelerometer, motor driver, Arduino and battery are all secured using double-sided tape.

The Arduino is installed on a special shelf that rises up from the floor at an angle.  This allows a USB cable to be attached to it for programming after assembly.

The battery is taped both against the floor of the robot - and the flat section of wall behind it.  There is clearance under the flat wall for a zip-tie if more security is desired (good idea).

The power switch, LED, capacitor and resistors are all held in place with hot glue.

The motor is held into place using zip ties.

This all seems to work - at least for testing purposes.  You might have better / more robust ideas.

<table><tr><td>
<img src="../media/bottom.jpg" alt="drawing" width="380"/></td><td>

<img src="../media/programming.jpg" alt="drawing" width="380"/></td>
</tr>
</table>


## Detachable Weapon

A shell option without a built-in weapon is provided.  Building the robot without a weapon is a good way to test with reduced "drama."

This shell includes a mounting hole so that a weapon may be attached later using a press-fit pin.  No assurances are given about the robustness of this approach.  Using hot-glue to re-enforce weapon attachment may prove useful.

<table><tr><td>
<img src="../media/weapon_on.png" alt="drawing" width="380"/></td><td>

<img src="../media/weapon_off.png" alt="drawing" width="380"/></td>
</tr>
</table>


## Motor 
A 45-turn 540-sized "sealed endbell" hobby motor is used.  [Injora](https://www.amazon.com/INJORA-Brushed-Tracks-AXI03007-Parts%EF%BC%8845T%EF%BC%89/dp/B08X21JCGZ/?th=1) is one of many common brands.

The number of turns determines how fast the motor rotates for a given voltage.  Lower turn motors spin faster and draw more current.  A 45-turn motor is considered a slower "torque / crawler" motor.  

Using a lower turn motor has not been tested - and may overtax the MOSFET.

The 30SQ045 Schottky diode must be soldered across the motor leads as indicated in the schematic - or the MOSFET will quickly be destroyed by back-voltage.


## Wheel Hub
The wheel hub is specified as 3mm, where the motor has a shaft of 3.175mm (1/8").  It will need to be very slightly drilled out.
<div align="center">

<img src="../media/wheel_hub.jpg" alt="drawing" width="400"/>
</div>

## Battery
The [7.4v 900mAh battery](https://www.amazon.com/dp/B09BYVNH2N?psc=1&ref=ppx_yo2ov_dt_b_product_details) used for the test build is specified as 53x29x16mm (46 grams).  The key dimension is that it needs to be < 30mm wide in order to fit as oriented.

Runtime on a full battery charge is over 6 minutes.  A somewhat smaller battery could probably be used.

Upgrading to a 11.1v battery may be possible - but some preliminary tests suggested it may be too much for the MOSFET to handle.  A dedicated voltage regulator is recommended for voltages over 7.4v.

## Schematic

Wire it up like this...
<div align="center">

<img src="../openmelt_schematic/schematic.jpg" alt="drawing" width="600"/>
</div>

## Wiring Things (Wire Wrap)
Using ["wire wrap"](https://learn.sparkfun.com/tutorials/working-with-wire/how-to-use-a-wire-wrap-tool) for all signal and low-power connections is recommended.  When done properly wire-wrap connections are reliable and provide strain relief.  The small gauge wires minimize the stress put on connections under the stresses experienced by a melty bot.

<div align="center">
<img src="../media/wire_wrap.png" alt="drawing" width="300"/>
</div>

Wire wrapping has fallen out-of-fashion - but there is a reason they used this approach for the [Apollo Guidance Computer](http://www.righto.com/2019/07/software-woven-into-wire-core-rope-and.html).

If using another wiring method - just keep in mind the high forces this robot will experience.  Temporary "Dupont wire" jumpers are not likely to work reliably.

## Heading LED / Visibility
The heading LED and resistor are hot glued into the upper-left corner of the robot.  

Glue the LED close to the top of the shell (away from the floor).

The lid / shell edge can potentially obscure LED visibility.  Mounting the LED so it slightly protrudes above the shell edge (flush with the lid) can help with this.

Be sure to test with the lid on to verify good LED visbility.

Make sure to glue the resistor to a surface so that it doesn't place strain on the LED or wiring.

<div align="center">
<img src="../media/led_mount.jpg" alt="drawing" width="300"/>
</div>

## Motor Driver

The RFP30N06LE is a widely available N-channel MOSFET.  The original FairChild / onsemi version seems discontinued - but many manufacturers still make versions of it.

Cut a small piece of PCB blank or thin copper sheet - and solder the tab of the RFP30N06LE to it.  This will provide a crude heatsink.

The tab is electrically equivalent to the middle pin.  The motor may be directly connected to the copper PCB surface (the middle pin may be cut off).

The PCB blank / copper sheet provides adequate surface area to be mounted using double-sided tape.

<div align="center">
<img src="../media/motor_driver.jpg" alt="drawing" width="240"/>
</div>


## Battery Voltage Monitor
Two resistors (10k and 100k Ohm) as shown in the schematic are required to create the 10:1 voltage divider needed for battery monitoring / alarm.

These can be hot-glued someplace convenient in the chassis.

The voltage monitor is optional - but a good idea to avoid over-discharging your battery.

See BATTERY\_ALERT\_ENABLED and BATTERY\_ADC\_WARN\_VOLTAGE\_THRESHOLD in [melty_config.h](../openmelt/melty_config.h) for options.


## Deploying Firmware

Get the latest version of the [Arduino IDE](https://www.arduino.cc/en/software).

You will need to also download and install the [Adafruit SleepyDog](https://github.com/adafruit/Adafruit_SleepyDog) and [SparkFun_LIS331](https://github.com/sparkfun/SparkFun_LIS331_Arduino_Library) libraries.

Select board type as "Arduino Micro" - and you should be ready to deploy the firmware.

The constants in [melty_config.h](../openmelt/melty_config.h) are intended to reflect the test platform - so the interactive configuration process should be fairly smooth.

<div align="center"><img src="../media/programming.jpg" alt="drawing" width="380"/></div>


## Optional Motor Modification

A 1-wheel robot results in a constant pulling force on the motor as it rotates.  This will cause the motor shaft to become loose over time (specifically - there is an internal shim on the shaft that slips).  These motors tend to still run - but are noisy, slower and less efficient.

One solution is to just replace the motor when it gets worn-out.  At about $15 - this is a pretty good option.

<div align="center">
<img src="../media/motor_shaft.jpg" alt="drawing" width="600"/>
<p>Worn motor showing ~2mm play in shaft</p></div>

A modification that seems to help with this problem is placing a 3.2mm inner-diameter plastic washer on the rear-end of the shaft - and then soldering a metal 3.2mm inner-diameter washer on top of it to sandwich it in place.

<div align="center">
<img src="../media/motor_hack.jpg" alt="drawing" width="400"/>
<p>Motor with soldered retaining washer / plastic glide washer</p>
</div>

A high temperature soldering gun and use of flux will help assure a good solder bond is quickly made - and reduce the chance of completely melting the plastic washer.

For the test build these parts were sourced from an already damaged 540 motor.

These parts from McMaster Carr have not been tested - but may provide similar functionality:  
[https://www.mcmaster.com/95395A104/](https://www.mcmaster.com/95395A104/)  
[https://www.mcmaster.com/93785A821/](https://www.mcmaster.com/93785A821/)


