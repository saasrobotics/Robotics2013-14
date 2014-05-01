#pragma config(Sensor, S1,     HTPB,                sensorI2CCustom9V)
//Arduino to Hitechnic Protoboard
// Wiring:
//  Arduino   to     Hitechnic
//    D0                A0
//    D1                A1
//    D2                A2
//    D3                A3
//    D4                A4
//    D5                B0
//    D6                B1
//    D7                B2
//    D8                B3
//////////////////////////////////

#include "drivers/hitechnic-protoboard.h"

task main() {
  int _chVal = 0;  // analog input
  byte inputs = 0;  // all digital inputs
  int value = 0;

  nxtDisplayCenteredTextLine(0, "HiTechnic");
  nxtDisplayCenteredBigTextLine(1, "Proto");
  nxtDisplayCenteredTextLine(3, "Test 1");
  nxtDisplayCenteredTextLine(5, "Connect HTPB");
  nxtDisplayCenteredTextLine(6, "to S1");

  wait1Msec(2000);
  // Setup all the digital IO ports as outputs (0xFF)
  if (!HTPBsetupIO(HTPB, 0xFF)) {
    nxtDisplayTextLine(4, "ERROR!!");
    wait1Msec(2000);
    StopAllTasks();
  }

  while(true) {
    value = 0;
  	eraseDisplay();
    // get the value for ADC channel 0, we want a 10 bit answer
		int j = 0;
		for(int i=0; i<5; i++)
		{

				_chVal = HTPBreadADC(HTPB, j, 10);
				switch(j) {
					case 0:  nxtDisplayTextLine(0, "A0: %d", _chVal);
					if(_chVal>512) value+=(1<<8);
					break;
					case 1:  nxtDisplayTextLine(1, "A1: %d", _chVal);
					if(_chVal>512) value+=(1<<7);
					break;
					case 2:  nxtDisplayTextLine(2, "A2: %d", _chVal);
					if(_chVal>512) value+=(1<<6);
					break;
					case 3:  nxtDisplayTextLine(3, "A3: %d", _chVal);
					if(_chVal>512) value+=(1<<5);
					break;
					case 4:  nxtDisplayTextLine(4, "A4: %d", _chVal);
					if(_chVal>512) value+=(1<<4);
					break; }

    	  j++;

  }
  inputs = HTPBreadIO(HTPB, 0x3F);
  nxtDisplayTextLine(5, "D: 0x%x", inputs);
  value+=(inputs&0x01)<<3;
  value+=(inputs&0x02)<<1;
  value+=(inputs&0x04)>>1;
  value+=(inputs&0x08)>>3;
  nxtDisplayBigTextLine(6, "%d", value);
  wait1Msec(100);
}
}
/*
 * $Id: hitechnic-protoboard-test1.c 133 2013-03-10 15:15:38Z xander $
 */
