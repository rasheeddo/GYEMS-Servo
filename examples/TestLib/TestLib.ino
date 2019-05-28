/***************************************************************
* Arduino GYEMS RMD-L-70 Servo Motor Library - Version 1.0
* by Rasheed Kittinanthapanya
* a simple RS485 communication between GYEMS servo and Arduino

* 
* Example Program: Control the servo in each mode by uncomment 
*                  the command you want to try on
***************************************************************/

#include <GYEMS.h>

int servo1 = 1;     //motorID
float OutputDeg;
float OutputRPM;
float averageDPS;

GYEMS GS(servo1);

void setup() {

  // Initialized serial0 for upload and print data out to screen
  Serial.begin(9600);
  Serial.println("GYEMS Servo was initialized");

  // Uncomment one of these control you want to test with
  GS.SpeedControl(270);
  //GS.PositionControlMode1(180);
  //GS.PositionControlMode2(0,90);
  //GS.PositionControlMode3(45,0x00);
  //GS.PositionControlMode4(90,360,0x01);

}

void loop() {

  //OutputDeg = GS.GetCurrentDEG();       // uncomment to get current angle, if using PositionControlMode1, 2, 3, or 4
  averageDPS = GS.GetAverageSpeed();  // uncomment to get average speed, if using SpeedControl
  
  // Open Serial Monitor to see the value printing out...
  //Serial.println(OutputDeg);      // uncomment to print current angle, if using PositionControlMode1, 2, 3, or 4
  Serial.println(averageDPS);   // uncomment to print average speed, if using SpeedControl
  
  //delay(1);
  
}
