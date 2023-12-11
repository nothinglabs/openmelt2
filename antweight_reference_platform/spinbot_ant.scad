weaponPartOfBody = false;   //render the weapon and the body together

renderBotBody = true;     //render the main shell or not
renderLid = false;           //render the lid of not
positionLidForPrint = true;  //if false - lid renders on top of the bot

renderWeapon = true;
renderWeaponPin = true;        

includeWeaponMountHole = weaponPartOfBody ? false : true;

renderArduinoShelf = true;  //HARDCODED shelf for Arduino to make USB port accessable while installed in bot
includeBatteryWall = true;     //parameters for this are HARDCODED
makeTapHoles = true;

//battery wall, weapon, electronics all have hard coded coordinates / sizes (need to tweak functions manually)
renderElectronics = true;
renderMotor = true;
renderWheel = true;

botHeight = 33;
botDiameter = 138;
botShellThickness = 5; 
floorThickness = 3; 
lidThickness = 1; 

motorDepthBelowFloorSurface = 5;    //motor mounts this deep below floor

wheelDiameter = 44;   
wheelWidth = 12.5;
wheelDistanceFromMotorWall = 8;

motorWallThickness = 3; 
motorWallOffset = 28;   //offset of motor wall relative to center of bot

motorDiameter = 37;  //bumped up by 1mm to assure easy mount
motorBushingDiameter = 13; 
motorBushingHeight = 2; 
motorLength = 51; //bumped up by 1mm to assure easy mount

motorMountHeight = 7;
motorMountLength = motorLength + 4; 
motorMountWidth = motorDiameter + 4; 
motorMountOffset = -2;

weaponMountHoleDiameter = 12;
weaponPinExtraLength = 5;
weaponPinDiameterReduce = 1;  //reduces diameter of weapon pin to account for 3d printing kerf

weaponTopVerticalExtension = weaponPartOfBody ? 0 : 4;  //weapon extends this much above shell
weaponBottomVerticalExtension = weaponPartOfBody ? 0 : 1.5;  //weapon extends this much below shell
weaponSlotExtraWidth = weaponPartOfBody ? 0 : 1.0;  //increases width of weapon slot (mm) to account for 3d printing kerf
weaponThickNess = 19;
weaponWidth = 40;
weaponInset = 2;


lidMotorCutWidthAdjustment = 1.5; //adjusts height of motor cut in lid to allow easy clearance (as if motor has been moved upwards by this many mm)

motorMountHoleOffset = 6;
motorMountHoleDiameter = 5.25;

lidScrewHoleDiameter = 2.2;        //diameter for self-tapping screws in print
lidScrewHoleDepth = 10; 

ledAndPowerHoleDiameter = 4;

motorWallWidth = (2 * ((botDiameter / 2)^2 - (motorWallOffset+motorWallThickness)^2)^0.5) - (botShellThickness);

powerHoleOffsetAngle = 140;
ledHoleOffsetAngle = -24;


$fn = 50;

if (renderLid) lid();

if (renderWeapon) {
    if (!weaponPartOfBody) color([1.0,0.0,0.0]) weapon();
    else weapon();
}

if (renderWeaponPin && !weaponPartOfBody) weaponPin();

if (renderBotBody) {

    motorWall();

    if (renderArduinoShelf) arduinoShelf();
    if (renderElectronics) dummyElectronics();

    //HARDCODED battery wall
    if (includeBatteryWall) {
        translate([-33,58,floorThickness + 3]) rotate ([90,0,0]) cube([66,botHeight - 8,3]);
    }

    if (renderMotor) {
        translate ([0, motorMountOffset + ((motorMountLength - motorLength) / 2),floorThickness])
           color([0.5,0.5,0.5]) motor();
    }

    if (renderWheel) {
        translate ([0,motorMountOffset,floorThickness - motorDepthBelowFloorSurface])
            color([0.2,0.2,0.2]) wheel();
    }
    

    //motor mount
    difference() {
        translate ([0,motorMountOffset,floorThickness]) motorMount();
        makeZipTieHoles();
    }

    //actual main body
    difference() {                

        mainShell(botHeight);
        makeZipTieHoles();

        translate ([0, motorMountOffset + ((motorMountLength - motorLength) / 2),floorThickness])
            motor();

        if (makeTapHoles) makeTapHoles();   
        
        if (includeWeaponMountHole) weaponMountHole();
    }

}

module mainShell(height) {
    difference() {
        cylinder(height,botDiameter / 2,botDiameter / 2);
        translate ([0,0,floorThickness]) cylinder(height, (botDiameter / 2) - botShellThickness,(botDiameter / 2) - botShellThickness);
        wheelWellCut();
    }

}


module motorWall() {
    difference() {
        translate([motorWallWidth / - 2,motorWallOffset * -1,0]) rotate ([90,0,0])
            cube([motorWallWidth,botHeight, motorWallThickness]);

            translate([motorBushingDiameter / -2,motorWallOffset * -1, motorMountHeight + floorThickness])
                rotate ([90,0,0])
                    cube([motorBushingDiameter, botHeight, motorWallThickness * 1]);
        if (makeTapHoles) makeTapHoles();

    }
}

module wheelWellCut() {
    intersection() {
        //remove wall thickness for top piece
        translate ([0,0,0]) cylinder(botHeight, (botDiameter / 2) - botShellThickness, (botDiameter / 2) - botShellThickness);
        translate([botDiameter / -2,(motorWallOffset * -1) - motorWallThickness,0])
            rotate ([90,0,0]) cube([botDiameter,botHeight,botDiameter]);
    }
}

module arduinoShelf()
{
    translate([46,19,15]) rotate ([-21,0,175])  cube([20,38,2]);
    translate([46,19,0]) rotate ([0,0,175]) translate([0,13,0]) cube([20,2,10]);
}


module dummyElectronics() {
    //LED
    color([0.1,0.1,0.8]) translate([56,-26,botHeight -5]) rotate ([0,0,0])  cylinder(d = 5, h= 5);

    //capacitor
    color([0.3,0.3,0.3]) translate([56,-3,floorThickness]) rotate ([0,0,0])  cylinder(d = 15, h= 25);

    //battery
    color([1.0,0.5,0.5]) translate([-34,54,floorThickness]) rotate ([90,0,0])  cube([53,29,16]);

    //motor driver (Mosfet + PCB heatsink)
    color([1.0,0.5,0.2]) translate([-55,24,floorThickness]) rotate ([90,0,0])  cube([18,15,18]);

    //rx placeholder (Flysky FS-iA6 Receiver)
    color([0.1,0.5,0.5]) translate([60,20,floorThickness]) rotate ([0,0,127])  cube([40,21.1,7.5]);

    //Adafruit ItsyBitsy 32u4
//    translate([45,18,15]) rotate ([-21,0,175])  cube([18,35,13]);
    
        //arduino micro 
    color([0.1,0.1,0.5])translate([45,23,18]) rotate ([-21,0,175])  cube([18,48,13]);


    //accelerometer (Adafruit H3LIS331)
    color([0.1,0.4,0.5]) translate([-52,-20,floorThickness]) rotate ([0,0,0])  cube([26,18,15]);
    
}


module lid() {
    translate([0,positionLidForPrint ? botDiameter + 1 : 0,positionLidForPrint ? 0: botHeight]) rotate([0, 0, positionLidForPrint ? 90 : 0]){
        difference() {        
            cylinder(lidThickness,botDiameter / 2,botDiameter / 2);
            if (makeTapHoles) {
                translate([0,0,(botHeight * -1) + lidThickness])
                makeTapHoles();   
            }
            
            translate([0,0,(botHeight * -1) + lidThickness])
            makeLedAndPowerHoles();

            wheelWellCut();
            
                translate ([0, motorMountOffset + ((motorMountLength - motorLength) / 2),floorThickness + (botHeight * -1) + lidMotorCutWidthAdjustment])
        motor();
                
            translate([botDiameter / -2,(motorWallOffset * -1) - motorWallThickness,0])
                rotate ([90,0,0]) cube([botDiameter,botHeight,botDiameter]);

        }
    }
}

module makeZipTieHoles(){
        holeOutset = (motorDiameter / 2) + 2;
                
        translate([holeOutset,motorMountOffset + ((motorLength /  2) - motorMountHoleOffset),0]) makeZipTieHole();
        translate([holeOutset * -1,motorMountOffset + ((motorLength / 2) - motorMountHoleOffset),0]) makeZipTieHole();

        translate([holeOutset,motorMountOffset ,0]) makeZipTieHole();
        translate([holeOutset * -1,motorMountOffset,0]) makeZipTieHole();

        translate([holeOutset,motorMountOffset + (-1 * ((motorLength / 2)) + motorMountHoleOffset),0]) makeZipTieHole();
        translate([holeOutset * -1,motorMountOffset + (- 1 * ((motorLength / 2)) + motorMountHoleOffset),0]) makeZipTieHole();
}

module makeZipTieHole(){
    rotate ([0,0,90]) cylinder(botHeight,motorMountHoleDiameter / 2,motorMountHoleDiameter / 2);
}

module weaponPin() {
    translate ([0,(botDiameter * - 0.5) + weaponInset,(floorThickness - motorDepthBelowFloorSurface) + motorDiameter / 2])
    rotate ([90,0,0])
    cylinder(weaponThickNess + weaponPinExtraLength,(weaponMountHoleDiameter - weaponPinDiameterReduce) / 2, (weaponMountHoleDiameter - weaponPinDiameterReduce) / 2, center = true);
}

module weapon(){
    difference() {
        translate ([0,(botDiameter * - 0.5) + weaponInset,(floorThickness - motorDepthBelowFloorSurface) + motorDiameter / 2])
        rotate ([90,0,0])
        translate ([weaponWidth * -.5,(botHeight * -.5) - weaponBottomVerticalExtension,weaponThickNess * -.5])

        cube([weaponWidth,botHeight + weaponTopVerticalExtension + weaponBottomVerticalExtension, weaponThickNess], center = false);
        translate([0,0,weaponBottomVerticalExtension * -1]) {
            mainShell(botHeight + weaponBottomVerticalExtension);

            translate([0,weaponSlotExtraWidth * -1, 0]) mainShell(botHeight + weaponBottomVerticalExtension);
        }



        if (includeWeaponMountHole) weaponMountHole();
    }
}

module weaponMountHole(){
    translate ([0,(motorWallOffset * -1) - motorWallThickness,(floorThickness - motorDepthBelowFloorSurface) + motorDiameter / 2])
    rotate ([90,0,0])
    cylinder(botDiameter,weaponMountHoleDiameter / 2, weaponMountHoleDiameter / 2);
}

module makeTapHoles(){
    for (offsetAngle = [0 : 60 : 200]) {
        rotate ([0, 0, offsetAngle])
        translate ([(botDiameter - botShellThickness) / 2, 0, botHeight - lidScrewHoleDepth])
        makeM3TapHole();
    }    
    translate ([0, 0, botHeight - lidScrewHoleDepth]) {

        translate([botDiameter / -3, ( motorWallOffset * -1) - motorWallThickness / 2,0])
        makeM3TapHole();
    
            translate([botDiameter / 3, ( motorWallOffset * -1) - motorWallThickness / 2,0])
        makeM3TapHole();
    }


}



module makeLedAndPowerHoles(){
    for (offsetAngle = [ledHoleOffsetAngle, powerHoleOffsetAngle]) {
        rotate ([0, 0, offsetAngle])
        translate ([((botDiameter  / 2) - (botShellThickness + ledAndPowerHoleDiameter)), 0, botHeight - lidScrewHoleDepth])
        rotate ([0,0,90]) cylinder(lidScrewHoleDepth, ledAndPowerHoleDiameter, ledAndPowerHoleDiameter);
    }    
}


module makeM3TapHole(){
    rotate ([0,0,90]) cylinder(lidScrewHoleDepth,lidScrewHoleDiameter / 2,lidScrewHoleDiameter / 2);
}


module motorMount() {
    difference() {
        translate ([0, (motorMountLength / 2) + motorWallOffset * -1, motorMountHeight / 2])
        cube ([motorMountWidth, motorMountLength, motorMountHeight], center = true);
        translate ([0, (motorMountLength - motorLength) / 2 ,0]) motor();
    }
}


module motor() {
    translate ([0,motorLength + (motorWallOffset * -1),(motorDiameter / 2) - motorDepthBelowFloorSurface]) 
        rotate ([90,0,0]) {        
            cylinder (motorLength,motorDiameter / 2, motorDiameter / 2);
            translate ([0,0,motorBushingHeight])
                cylinder (motorLength,motorBushingDiameter / 2, motorBushingDiameter / 2);

        };

}

module wheel() {
    translate ([0,(motorWallOffset * -1) - wheelDistanceFromMotorWall ,(motorDiameter / 2)]) 
    rotate ([90,0,0]) {        
        cylinder (wheelWidth,wheelDiameter / 2, wheelDiameter / 2);
    }
}