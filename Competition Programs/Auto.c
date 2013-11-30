#pragma config(Sensor, S1,     ,               sensorI2CCustom)
#pragma config(Sensor, S2,     IR,             sensorI2CCustom)
#pragma config(Sensor, S3,     Color,          sensorI2CCustom)
#pragma config(Sensor, S4,     ,               sensorTouch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../libraries/Servos.h"
#include "../libraries/Arm.c"
#include "../libraries/Motors.h"
#include "JoystickDriver.c"

task UpdateArm;

int ProgramDelay;
//POSSIBLE VALUES: any int
string RampOrientation;
//POSSIBLE VALUES: "Left", "Right"
string RampY;
//POSSIBLE VALUES: "Half", "On", "Over"
string RampX;
//POSSIBLE VALUES: "Left", "Middle", "Right"
bool Rotate90;


void Move(int RightPower, int LeftPower)
{
	Motors_SetSpeed(S1, 2, 1, RightPower);
	Motors_SetSpeed(S1, 2, 2, -LeftPower);
}

void CalculateRampOrientation()
{
	//
	RampOrientation = "Right";
	eraseDisplay();
	nxtDisplayString(1, "Which side is");
	nxtDisplayString(2, "the ramp on?");
	nxtDisplayBigStringAt(30, 20, "Right");
	nxtDisplayStringAt(0, 10, "Left");

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1)
		{
			RampOrientation = "Right";
			eraseDisplay();
			nxtDisplayString(1, "Which side is ");
			nxtDisplayString(2, "the ramp on?");
			nxtDisplayBigStringAt(30, 20, "Right");
			nxtDisplayStringAt(0, 10, "Left");
		}
		if(nNxtButtonPressed == 2)
		{
			RampOrientation = "Left";
			eraseDisplay();
			nxtDisplayString(1, "Which side is ");
			nxtDisplayString(2, "the ramp on?");
			nxtDisplayStringAt(60, 10, "Right");
			nxtDisplayBigStringAt(0, 20, "Left");
		}
	}
}

task main()
{
	//no debug diagnostics
	bDisplayDiagnostics = false;

	eraseDisplay();
	nxtDisplayString(1, "Which side is");
	nxtDisplayString(2, "the ramp on?");

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1)
		{
			eraseDisplay();
			nxtDisplayString(1, "Which side is ");
			nxtDisplayString(2, "the ramp on?");
			nxtDisplayBigStringAt(30, 20, "Right");
			nxtDisplayStringAt(0, 10, "Left");
		}
		if(nNxtButtonPressed == 2)
		{
			eraseDisplay();
			nxtDisplayString(1, "Which side is ");
			nxtDisplayString(2, "the ramp on?");
			nxtDisplayStringAt(60, 10, "Right");
			nxtDisplayBigStringAt(0, 20, "Left");
		}
	}
while(true){}
	sleep(500);

	eraseDisplay();
	nxtDisplayString(1, "Program delay?");

	while(nNxtButtonPressed != 3)
	{
		if(nNxtButtonPressed == 1)
		{
			eraseDisplay();
			nxtDisplayString(1, "Program delay?");
			//WaitTime++;
			nxtDisplayBigStringAt(30, 20, "%i", WaitTime);
			sleep(200);

		}
		else if(nNxtButtonPressed == 2)
		{
			eraseDisplay();
			nxtDisplayString(1, "Program delay?");
			if(WaitTime > 0)
			{
				//WaitTime--;
			}
			nxtDisplayBigStringAt(30, 20, "%i", WaitTime);
			sleep(200);
		}
	}

	eraseDisplay();
	nxtDisplayString(1, "Program Ready.");
	nxtDisplayString(2, "Waiting for Start");
	nxtDisplayString(3, "%i", Multiplier);

	//starts tasks
	StartTask(UpdateArm);

	waitForStart();

	sleep(WaitTime * 1000);

	eraseDisplay();

	//sets arm to go up
	Arm_SetSpeed(-100);
	sleep(2500);

	//moves forward
	Move(40, 40);
	sleep(600);
	Move(0, 0);

	//stop arm
	Arm_SetSpeed(0);

	//spit out block
	Servos_SetPosition(S1, 3, 1, 256);
	sleep(500);
	Servos_SetPosition(S1, 3, 1, 128);

	//moves backward
	Move(-40, -40);
	sleep(200);
	Move(0, 0);

	//turn
	Move(40 * Multiplier, -40 * Multiplier);
	sleep(700);
	Move(0, 0);

	//moves arm down
	Arm_SetSpeed(40);

	//moves back
	Move(-40, -40);
	sleep(800);

	//turn onto ramp
	if(Multiplier == 1)
	{
		Move(-20, -80);
	}
	if(Multiplier == -1)
	{
		Move(-80, -20);
	}
	sleep(1000);
	Move(0, 0);

	//moves backward
	Move(-40, -40);
	sleep(1200);
	Move(0, 0);
}

task UpdateArm()
{
	while(true)
	{
		Arm_Update();
	}
}