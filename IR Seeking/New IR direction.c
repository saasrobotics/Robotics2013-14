#pragma config(Sensor, S1,     ,               sensorI2CCustom)
#pragma config(Sensor, S2,     IR,             sensorI2CCustom)
#pragma config(Sensor, S3,     Color,          sensorI2CCustom)
#pragma config(Sensor, S4,     ,               sensorTouch)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../libraries/Servos.h"
#include "../libraries/Arm.c"
#include "../libraries/Motors.h"
#include "LineOfBestFit.c"
#include "hitechnic-colour-v1.h"

int A, B, C, D, E = 0;
string State = "Searching";
int ColorNumber;
int IRSlope;
int Encoder;
int InitialEncoder;
task UpdateVariables;
task UpdateIR;

void Move(int RightPower, int LeftPower)
{
	Motors_SetSpeed(S1, 2, 1, RightPower);
	Motors_SetSpeed(S1, 2, 2, -LeftPower);
}

void FollowLine()
{
	//on line
	if(ColorNumber == 2 || ColorNumber == 8)
	{
		Move(24, 30);
	}
	//off line
	else
	{
		Move(30, 24);
	}
}

task main()
{
	//starts tasks
	StartTask(UpdateVariables);
	StartTask(UpdateIR);

	//sets up initial encoder
	InitialEncoder = I2C_GetEncoderPosition(S1, 2, 2);

	//sets arm to go up
	Arm_SetSpeed(-100);

	while(true)
	{
		if(State == "Searching")
		{
			Move(15, 15);

			if(ColorNumber == 2 || ColorNumber == 8)
			{
				State = "Following Line";
			}

			if(IRSlope < 0)
			{
				State = "Found";
			}
		}

		if(State == "Following Line")
		{
			if(IRSlope < 0)
			{
				State = "Found";
			}

			FollowLine();
		}

		if(State == "Found")
		{
			//if the robot is at one of the near baskets
			if (Encoder - InitialEncoder < (-2 * 1440))
			{
				State = "Moving Short";
			}

			//if the robot is at a far backet
			else
			{
				State = "Moving Long";
			}
		}

		if(State == "Moving Short")
		{
			//moves robot forward
			Move(10, 10);
			sleep(1000);
			Move(0, 0);

			State = "Placing Block";
		}

		if(State == "Moving Long")
		{
			//moves robot forward
			Move(10, 10);
			sleep(2000);
			Move(0, 0);

			State = "Placing Block";
		}

		if(State == "Placing Block")
		{
			//turns robot right
			Move(-10, 10);
			sleep(1000);

			//backs robot up
			Move(-10, -10);
			sleep(500);

			//stops motors
			Move(0, 0);

			//spits block out by turning servo
			Servos_SetPosition(S1, 3, 1, 0);
			sleep(1000);

			//Moves Robot forwars
			Move(10, 10);
			sleep(500);

			//turns robot left
			Move(10, -10);
			sleep(1000);

			//stops motors
			Move(0, 0);

			while(true)
			{
			}
		}
	}
}

task UpdateIR()
{
	while(true)
	{
		//puts in temp variable
		int TempIR = LineOfBestFit_Slope(10);

		//changes global, takes full CPU
		hogCPU();
		IRSlope = TempIR;
		releaseCPU();

		//lets other tasks run
		sleep(20);
	}
}

task UpdateVariables()
{
	while(true)
	{
		//writes the state to the screen
		nxtDisplayString(1, State);
		//updates the arm
		Arm_Update();
		//computes and stores in temp variables
		int TempEncoder = I2C_GetEncoderPosition(S1, 2, 2);
		int TempColor = HTCSreadColor(Color);

		//take CPU and set variables
		hogCPU();
		Encoder = TempEncoder;
		ColorNumber = TempColor;
		releaseCPU();

		//let other tasks run
		sleep(20);
	}
}