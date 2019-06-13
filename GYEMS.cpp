/***************************************************************
* Arduino GYEMS RMD-L-70 Servo Motor Library - Version 1.0
* by Rasheed Kittinanthapanya
* a simple RS485 communication between GYEMS servo and Arduino
***************************************************************/

#include "Arduino.h"
#include "GYEMS.h"

GYEMS::GYEMS(int ID)
{
  _ID = (byte)ID;                                       // simply convert int to hex
  //Serial.print("_ID,HEX   ");
  //Serial.println(_ID,HEX);
  pinMode(RS485_EN, OUTPUT);                            // set enable pin for RS485 module as pin 2 of Arduino

  
  //delay(100);

  Serial1.begin(115200);
  pinMode(RS485_EN,OUTPUT);
  digitalWrite(RS485_EN,LOW);
}

void GYEMS::Int16ToByteData(unsigned int Data, unsigned char StoreByte[2])
{
  // unsigned int can store 16 bit int 
  StoreByte[0] = (Data & 0xFF00) >> 8;                  //High byte, most right of HEX
  StoreByte[1] = (Data & 0x00FF);                       //Low byte, most left of HEX
}

void GYEMS::Int32ToByteData(unsigned long Data, unsigned char StoreByte[4]){
  
  // unsigned long can store 32 bit int 
  StoreByte[0] = (Data & 0xFF000000) >> 24;             //High byte, most right of HEX
  StoreByte[1] = (Data & 0x00FF0000) >> 16;
  StoreByte[2] = (Data & 0x0000FF00) >> 8;
  StoreByte[3] = (Data & 0x000000FF);                   //Low byte, most left of HEX
}

void GYEMS::Int64ToByteData(unsigned long long Data, unsigned char StoreByte[8]){
  
  // unsigned long long can store 64 bit int 
  StoreByte[0] = (Data & 0xFF00000000000000) >> 56;       //High byte, most right of HEX
  StoreByte[1] = (Data & 0x00FF000000000000) >> 48;
  StoreByte[2] = (Data & 0x0000FF0000000000) >> 40;
  StoreByte[3] = (Data & 0x000000FF00000000) >> 32;     
  StoreByte[4] = (Data & 0x00000000FF000000) >> 24;     
  StoreByte[5] = (Data & 0x0000000000FF0000) >> 16;
  StoreByte[6] = (Data & 0x000000000000FF00) >> 8;
  StoreByte[7] = (Data & 0x00000000000000FF);             //Low byte, most left of HEX
}

unsigned int GYEMS::Make12BitData(unsigned char loByte, unsigned char hiByte)
{
  unsigned int word;
  unsigned int Ang12Bit;

  word = (loByte & 0xFF) | ( (hiByte & 0xFF) << 8);       // construct 2 bytes data to a single 16 bits int
  Ang12Bit = map(word,0,16383,0,4095);                    // simply convert 16 bits to 12 bits data

  return Ang12Bit;
}

float GYEMS::ReadReply()
{
  int i = 0;
  bool ReadOK = false;
  unsigned char EncoderReply[8] = {0,0,0,0,0,0,0,0};
  unsigned int EncoderData = 0;
  float startTime;
  //Serial.println(Serial1.read());
  //while(Serial1.read() > 0);
  //Serial.println(Serial1.available());
  //Serial.println(ReadOK);
  while ((Serial1.available() > 0))                         // wait for incoming data on Rx
  {
    ReadByte = Serial1.read();                            // read incoming byte
    EncoderReply[i] = ReadByte;                           // store it in array
    //Serial.println(i);
    //Serial.println(EncoderReply[i],HEX);
    //delayMicroseconds(1000);
    i++;
    
    ReadOK = true;
  }
  Serial.print("i ");
  Serial.println(i);
  if (ReadOK == true)
  {
    //Serial.println("ReadReply");
    EncoderData = Make12BitData(EncoderReply[5],EncoderReply[6]); // byte5 and byte 6 are lo-byte and hi-hyte of encoder data
    /*
    startTime = micros();
    Serial.println("EncoderReply");
    Serial.println(EncoderReply[0],HEX);
    Serial.println(EncoderReply[1],HEX);
    Serial.println(EncoderReply[2],HEX);
    Serial.println(EncoderReply[3],HEX);
    Serial.println(EncoderReply[4],HEX);
    Serial.println(EncoderReply[5],HEX);
    Serial.println(EncoderReply[6],HEX);
    Serial.println(EncoderReply[7],HEX);
    period = (micros() - startTime)*0.000001;
    Serial.println(period, 7);
    */
    delayMicroseconds(2000);                      // THIS DELAY IS IMPORTANT to make reading two IDs not mix up
                                                  // similar with the one in GetCurrentDEG()
                                                  // above Serial.print() takes around 2.0~2.5ms and it solves the issue!
                                                  // so instead of print something out, i just mimic it as delay

    CurrentDeg = map(EncoderData,0,4095,0.0,360.0);       // simply convert 12bits data to degree
    //ReadOK = false;
    //Serial.println(CurrentDeg);
    //delayMicroseconds(500);
    //digitalWrite(RS485_EN,HIGH); 
  }

  return CurrentDeg;
}

float GYEMS::GetCurrentDEG()
{
  
  byte EncoderCommand = 0x90;
  byte DataLength = 0x00;
  byte DataCheckByte = Header + EncoderCommand + _ID + DataLength;
  float startTime;

  Serial.print("ID is   ");
  Serial.println(_ID,HEX);
  
  // send a command to servo to request a current position

  digitalWrite(RS485_EN,HIGH);                            // Pulling the pin 2 HIGH for sending via Tx
  delayMicroseconds(50);                                  // delay a bit before start sending...
  Serial1.write(Header);
  Serial1.write(EncoderCommand);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(DataCheckByte);
  Serial1.flush();
  digitalWrite(RS485_EN,LOW);                            // Pulling the pin 2 LOW for receving via Rx
  /*
  Serial.println("GetCurrentDEG");
  startTime = micros();
  Serial.println(Header,HEX);
  Serial.println(EncoderCommand,HEX);
  Serial.println(_ID,HEX);
  Serial.println(DataLength,HEX);
  Serial.println(DataCheckByte,HEX);
  period = (micros() - startTime)*0.000001;
  Serial.println(period, 7);
  */
  delayMicroseconds(800);                         // THIS DELAY IS IMPORTANT to make reading two IDs not mix up
                                                  // DO NOT CHANGE THIS VALUE!!
                                                  // above Serial.print() takes around 0.6~0.7ms and it solves the issue!
                                                  // so instead of print something out, i just mimic it as delay

  //startTime = micros();
  OutputDeg = ReadReply();                              // This function takes around 0.01 sec
  delayMicroseconds(800);                               // This delay will make the next incoming data not crash, when reading more than one servo 
  //period = (micros() - startTime)*0.000001;
  //Serial.println(period, 7);
 // Serial.print("OutputDeg    ");
  //Serial.println(OutputDeg,DEC);


  return OutputDeg;
}

float GYEMS::EstimateDPS()
{float OutputDeg;
  byte EncoderCommand = 0x90;
  byte DataLength = 0x00;
  byte DataCheckByte = Header + EncoderCommand + _ID + DataLength;
  
  // send a command to servo to request a current position
  digitalWrite(RS485_EN,HIGH);                            // Pulling the pin 2 HIGH for sending via Tx
  delayMicroseconds(50);                                  // delay a bit before start sending...
  Serial1.write(Header);
  Serial1.write(EncoderCommand);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(DataCheckByte);
  Serial1.flush();
  digitalWrite(RS485_EN,LOW);                            // Pulling the pin 2 LOW for receving via Rx

  theta2 = ReadReply();                            // current position
  t2 = micros();                                   // current time
  period = (t2 - t1)*0.000001;                     // difference of time 
  CurrentDPS = (theta2 - theta1)/period;           // velocity = difference of distance/difference of time

  if (abs(CurrentDPS) > MAXDPS)
  {
    CurrentDPS = LastDPS;                           // if there is a spike of velocity, reset it to previous value
  }

  t1 = t2;                                          // update previous time
  theta1 = theta2;                                  // update previous angle
  LastDPS = CurrentDPS;                             // udpate previous velocity

  return CurrentDPS;

}

float GYEMS::GetAverageSpeed()
{
  float Speed;
  float AccumSpeed;
  int sampler = 250;                                // a total sampling data for doing average
  float aveDPS;
  int i = 0;

  for(i=0; i < sampler;i++)
  { 
    Speed = EstimateDPS();                          // estimate current speed in degree per second
    //Serial.println(Speed);
    if(i==0)
    {
      AccumSpeed = Speed;
    }
    else
    {
      AccumSpeed = AccumSpeed + Speed;              // accumulating value
      //Serial.println(AccumSpeed);
    }
    delayMicroseconds(1000);                        // a small delay for stabilize the loop ***Without this, aveDPS will show 0.0, still don't understand why??***

  }
    
  aveDPS = AccumSpeed/sampler;                      // average speed
  //Serial.println(aveDPS);
  return aveDPS;

}

void GYEMS::SpeedControl(unsigned long DPS){
  
  // DPS is degree per second
  unsigned long SpeedLSB = DPS*100;
  byte SpeedCommand = 0xA2;
  byte DataLength = 0x04;
  byte FrameCheckSum = Header + SpeedCommand + DataLength + _ID;
  unsigned char SpeedByte[4];
  Int32ToByteData(SpeedLSB,SpeedByte);
  byte DataCheckByte = SpeedByte[3] + SpeedByte[2] + SpeedByte[1] + SpeedByte[0];

  digitalWrite(RS485_EN,HIGH);
  delayMicroseconds(50);
  Serial1.write(Header);
  Serial1.write(SpeedCommand);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(FrameCheckSum);
  Serial1.write(SpeedByte[3]);
  Serial1.write(SpeedByte[2]);
  Serial1.write(SpeedByte[1]);
  Serial1.write(SpeedByte[0]);
  Serial1.write(DataCheckByte);
  Serial1.flush();
  digitalWrite(RS485_EN,LOW);

  //ReadReply();
}

void GYEMS::PositionControlMode1(unsigned long long Deg)
{
  unsigned long long DegLSB = Deg*100;
  byte PositionCommand1 = 0xA3;
  byte DataLength = 0x08;
  byte FrameCheckSum = Header + PositionCommand1 + _ID + DataLength ;
  unsigned char PositionByte[8];
  Int64ToByteData(DegLSB,PositionByte);
  byte DataCheckByte = PositionByte[7] + PositionByte[6] + PositionByte[5] + PositionByte[4] + PositionByte[3] + PositionByte[2] + PositionByte[1] + PositionByte[0];

  digitalWrite(RS485_EN,HIGH);
  delayMicroseconds(50);
  Serial1.write(Header);
  Serial1.write(PositionCommand1);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(FrameCheckSum);
  Serial1.write(PositionByte[7]);
  Serial1.write(PositionByte[6]);
  Serial1.write(PositionByte[5]);
  Serial1.write(PositionByte[4]);
  Serial1.write(PositionByte[3]);
  Serial1.write(PositionByte[2]);
  Serial1.write(PositionByte[1]);
  Serial1.write(PositionByte[0]);
  Serial1.write(DataCheckByte);
  Serial1.flush();
  digitalWrite(RS485_EN,LOW);

  //ReadReply();
 
}

void GYEMS::PositionControlMode2(unsigned long long Deg, unsigned long DPS)
{
  unsigned long long DegLSB = Deg*100;
  unsigned long SpeedLSB = DPS*100;
  byte PositionCommand2 = 0xA4;
  byte DataLength = 0x0C;
  byte FrameCheckSum = Header + PositionCommand2 + _ID + DataLength ;
  unsigned char PositionByte[8];
  unsigned char SpeedByte[4];
  Int32ToByteData(SpeedLSB,SpeedByte);
  Int64ToByteData(DegLSB,PositionByte);
  byte DataCheckByte = PositionByte[7] + PositionByte[6] + PositionByte[5] + PositionByte[4] + PositionByte[3] + PositionByte[2] + PositionByte[1] + PositionByte[0] +  SpeedByte[3] + SpeedByte[2] + SpeedByte[1] + SpeedByte[0];

  digitalWrite(RS485_EN,HIGH);
  delayMicroseconds(50);
  Serial1.write(Header);
  Serial1.write(PositionCommand2);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(FrameCheckSum);
  Serial1.write(PositionByte[7]);
  Serial1.write(PositionByte[6]);
  Serial1.write(PositionByte[5]);
  Serial1.write(PositionByte[4]);
  Serial1.write(PositionByte[3]);
  Serial1.write(PositionByte[2]);
  Serial1.write(PositionByte[1]);
  Serial1.write(PositionByte[0]);
  Serial1.write(SpeedByte[3]);
  Serial1.write(SpeedByte[2]);
  Serial1.write(SpeedByte[1]);
  Serial1.write(SpeedByte[0]);
  Serial1.write(DataCheckByte);
  Serial1.flush();

  digitalWrite(RS485_EN,LOW);




  //ReadReply(); 

 
}

void GYEMS::PositionControlMode3(unsigned long Deg, byte Direction)
{
  unsigned long DegLSB = Deg*100;
  byte PositionCommand3 = 0xA5;
  byte DataLength = 0x04;
  byte FrameCheckSum = Header + PositionCommand3 + _ID + DataLength ;
  unsigned char PositionByte[4];
  Int32ToByteData(DegLSB,PositionByte);
  byte DataCheckByte = Direction + PositionByte[3] + PositionByte[2] + PositionByte[1];

  digitalWrite(RS485_EN,HIGH);
  delayMicroseconds(50);
  Serial1.write(Header);
  Serial1.write(PositionCommand3);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(FrameCheckSum);
  Serial1.write(Direction);
  Serial1.write(PositionByte[3]);
  Serial1.write(PositionByte[2]);
  Serial1.write(PositionByte[1]);
  Serial1.write(DataCheckByte);
  Serial1.flush();
  digitalWrite(RS485_EN,LOW);

  //ReadReply();

}

void GYEMS::PositionControlMode4(unsigned long Deg, unsigned long DPS, byte Direction)
{
  unsigned long DegLSB = Deg*100;
  unsigned long SpeedLSB = DPS*100;
  byte PositionCommand4 = 0xA6;
  byte DataLength = 0x08;
  byte FrameCheckSum = Header + PositionCommand4 + _ID + DataLength ;
  unsigned char PositionByte[4];
  unsigned char SpeedByte[4];
  Int32ToByteData(DegLSB,PositionByte);
  Int32ToByteData(SpeedLSB,SpeedByte);
  byte DataCheckByte = Direction + PositionByte[3] + PositionByte[2] + PositionByte[1] + SpeedByte[3] + SpeedByte[2] + SpeedByte[1] + SpeedByte[0];

  digitalWrite(RS485_EN,HIGH);
  delayMicroseconds(50);
  Serial1.write(Header);
  Serial1.write(PositionCommand4);
  Serial1.write(_ID);
  Serial1.write(DataLength);
  Serial1.write(FrameCheckSum);
  Serial1.write(Direction);
  Serial1.write(PositionByte[3]);
  Serial1.write(PositionByte[2]);
  Serial1.write(PositionByte[1]);
  Serial1.write(SpeedByte[3]);
  Serial1.write(SpeedByte[2]);
  Serial1.write(SpeedByte[1]);
  Serial1.write(SpeedByte[0]);
  Serial1.write(DataCheckByte);
  Serial1.flush();
  digitalWrite(RS485_EN,LOW);

  //ReadReply();

}