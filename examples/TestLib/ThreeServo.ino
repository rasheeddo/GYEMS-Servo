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
int servo2 = 2;
int servo3 = 3;

float OutputDeg = 0.0;
float OutputDeg2 = 0.0;
float OutputDeg3 = 0.0;
float OutputRPM;
float averageDPS;
float averageDPS2;
float averageDPS3;
bool read1;
bool read2;

GYEMS GS1(servo1);
GYEMS GS2(servo2);
GYEMS GS3(servo3);

void setup() {

  // Initialized serial0 for upload and print data out to screen
  Serial.begin(115200);
  Serial.println("GYEMS Servo was initialized");

  // Uncomment one of these control you want to test with
  //GS.SpeedControl(360);
  //GS2.SpeedControl(180);
  //GS3.SpeedControl(90);
  
  //GS.PositionControlMode1(180);
  
  GS1.PositionControlMode2(90,180);
  GS2.PositionControlMode2(180,360);
  GS3.PositionControlMode2(240,720);
  
  //GS.PositionControlMode3(60,0x00);
  //GS2.PositionControlMode3(90,0x00);
  
  //GS.PositionControlMode4(120,720,0x01);
  //GS2.PositionControlMode4(60,30,0x01);
  //GS3.PositionControlMode4(30,360,0x00);

  read1 = true;
  read2 = false;
}

void loop() {

  OutputDeg = GS1.GetCurrentDEG();       // uncomment to get current angle, if using PositionControlMode1, 2, 3, or 4
  //delay(100);
  OutputDeg2 = GS2.GetCurrentDEG();       // uncomment to get current angle, if using PositionControlMode1, 2, 3, or 4
  //delay(100);
  OutputDeg3 = GS3.GetCurrentDEG();       // uncomment to get current angle, if using PositionControlMode1, 2, 3, or 4
  
  //averageDPS = GS.GetAverageSpeed();  // uncomment to get average speed, if using SpeedControl
  //averageDPS2 = GS2.GetAverageSpeed();  // uncomment to get average speed, if using SpeedControl
  //averageDPS3 = GS3.GetAverageSpeed();  // uncomment to get average speed, if using SpeedControl
  
  
  // Open Serial Monitor to see the value printing out...
  Serial.print("Output Angle1 : ");
  Serial.println(OutputDeg);      // uncomment to print current angle, if using PositionControlMode1, 2, 3, or 4
  Serial.print("Output Angle2 : ");
  Serial.println(OutputDeg2);      // uncomment to print current angle, if using PositionControlMode1, 2, 3, or 4
  Serial.print("Output Angle3 : ");
  Serial.println(OutputDeg3);      // uncomment to print current angle, if using PositionControlMode1, 2, 3, or 4
  Serial.println("--------------------");

  //Serial.print("Average Speed1 : ");
  //Serial.println(averageDPS);   // uncomment to print average speed, if using SpeedControl
  //Serial.print("Average Speed2 : ");
  //Serial.println(averageDPS2);   // uncomment to print average speed, if using SpeedControl
  //Serial.print("Average Speed3 : ");
  //Serial.println(averageDPS3);   // uncomment to print average speed, if using SpeedControl
  //Serial.println("--------------------");

  
  delay(500); // a bit of delay to make the screen readable
  
}
