/***************************************************************
* Arduino GYEMS RMD-L-70 Servo Motor Library - Version 1.0
* by Rasheed Kittinanthapanya
* a simple RS485 communication between GYEMS servo and Arduino
***************************************************************/

#ifndef GYEMS_H
#define GYEMS_H

#include "Arduino.h"

#define RS485_EN 2
#define MAXDPS 30000

class GYEMS
{
public:
    GYEMS(int ID);
    //class constructor to initialize the pinMode and baudrate setting 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// Data Converting function /////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void Int16ToByteData(unsigned int Data, unsigned char StoreByte[2]);
    // Int16ToByteData: split an unsigned int into 2 bytes data
    // unsigned int stores 16 bits as an integer
    // Pass by reference using array StoreByte[2], this is output of the function

    void Int32ToByteData(unsigned long Data, unsigned char StoreByte[4]);
    // Int32ToByteData: split an unsigned long into 4 bytes data
    // unsigned long stores 32 bits as an integer
    // Pass by reference using array StoreByte[4], this is output of the function

    void Int64ToByteData(unsigned long long Data, unsigned char StoreByte[8]);
    // Int64ToByteData: split an unsigned long long into 8 bytes data
    // unsigned long long stores 64 bits as an integer
    // Pass by reference using array StoreByte[8], this is output of the function

    unsigned int Make12BitData(unsigned char loByte, unsigned char hiByte);
    // Mak12BitData: construct 2 bytes data from low byte and high byte into a single WORD (16 bits)
    // then convert that 16 bits data to 12 bits data  (for encoder data of RMD-S series)

    unsigned int Make14BitData(unsigned char loByte, unsigned char hiByte);
    // Mak14BitData: construct 2 bytes data from low byte and high byte into a single WORD (16 bits)
    // then convert that 16 bits data to 14 bits data  (for encoder data of RMD-L series)


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// Servo control function ///////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    float GetCurrentDEG();
    // GetCurrentDEG: a command to send data to the servo to request the current position
    // after a servo receive the command, it will reply back as a encoder data as 12bit (0-4095)
    // return an angle data in degree

    float EstimateDPS();
    // EstimateDPS: a command to send data to the servo to request the current position, then calculate the difference of time and position
    // this is myself simple algorithm, so if you have another idea how to compute speed, please replace by yours
    // return a speed in degree per second

    float GetAverageSpeed();
    // GetAverageSpeed: this function is related with EstimateDPS, it will do the average of the collecting speed to remove noisy data
    // again this is just a simple algorithm, if you have better idea, please feel free to change

    float ReadReply();
    // ReadReply: this function will wait for the servo to send the data back
    // output of this function will give us a current position of encoder as 12bit data (0-4095)
    // return an angle data in degree

    void MotorOff();
    // MotorOff: Turn off the motor and clear the motor running status and the previously received control commands.

    void MotorStop();
    // MotorStop: stop the motor but does not clear the motor running status and the previously received control command.

    void MotorRun();
    // MotorStop: continue run the motor from MotorStop(), control mode would be same as before stop

    void SetZero();
    // SetZero: set current motor position as zero position
    // NOTE from factory instruction manual
    // 1． The command will be valid after power on again.
	// 2． The command will write the zero point to the drive FLASH memory. Multiple writes will affect the chip life. 
	// It is not recommended to use it frequently

    void TorqueControl(unsigned int Torque);
    // TorqueControl: closed loop torque control
    // Input is the ratio of torque from -5000 to +5000, the actual torque depends on motor's model

    void SpeedControl(float DPS);
    // SpeedControl: this function is to control the speed of servo in degree per second (dps or DPS). max. value is 720DPS.
    // The direction of the motor is determined from the sign of input DPS

    void PositionControlMode1(unsigned long long Deg);
    // PositionControlMode1: The servo will run with maximum speed as config (720dps as default) to the desired position "Deg"
    // This is a multi turn mode, the input Deg can be from -3600deg to 3600deg (10rounds)

    void PositionControlMode2(unsigned long long Deg, unsigned long DPS);
    // PositionControlMode2: The servo will run with the specified speed as "DPS" to the desired angle "Deg"
    // This is a multi turn mode, the input Deg can be from -3600deg to 3600deg (10rounds), and max. speed is 720dps.

    void PositionControlMode3(unsigned long Deg, byte Direction);
    // PositionControlMode3: The servo will run with maximum speed as config (720dps as default) to the desired position "Deg" 
    // with the desired direction as "Direction", 0x00 for clockwise and 0x01 with counter clockwise

    void PositionControlMode4(unsigned long Deg, unsigned long DPS, byte Direction);
    // PositionControlMode4: The servo will run with the specified speed as "DPS" to the desired angle "Deg" 
    // with desired direction as "Direction", 0x00 for clockwise and 0x01 with counter clockwise


private:
    int _currentPosition;
    byte Header = 0x3E;
    byte _ID;
    
    unsigned char ReadByte = 0;
    float OutputDeg;
    float CurrentDeg;

    float t2;
    float t1 = 0.0;
    float period;
    float theta1 = 0.0;
    float theta2;
    float CurrentDPS;
    float LastDPS;
    

};

#endif
