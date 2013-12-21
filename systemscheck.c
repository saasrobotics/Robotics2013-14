#pragma config(Sensor, S1,     ,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

string textToAdd;

#include "libraries/I2C.h"
#include "libraries/Arm.c"

void addLog()
{
	string str = textToAdd;
	eraseDisplay();
	nxtDisplayString(0, str);
	writeDebugStreamLine(str);
}

void testEncoder(int daisychain, int motornumber)
{
	int encoderposition = I2C_GetEncoderPosition(S1, daisychain, motornumber);

	// if it's not 0 it must have moved sometime earlier, indicating that the encoder is working
	if (encoderposition != 0) return;

	I2C_SetMotorSpeed(S1, daisychain, motornumber, 10);
	Sleep(150);
	I2C_SetMotorSpeed(S1, daisychain, motornumber, 0);
	if (encoderposition == I2C_GetEncoderPosition(S1, daisychain, motornumber))
	{
		textToAdd = "FATAL: encoder test failed. aborting.";
		addLog();
		StopAllTasks();
	}
	textToAdd = "success: encoder test passed. continuing.";
	addLog();
}

void testMotor(int daisychain, int motornumber)
{
	I2C_SetMotorSpeed(S1, daisychain, motornumber, 100);
	Sleep(100);
	I2C_SetMotorSpeed(S1, daisychain, motornumber, 0);
}

void wait()
{
	textToAdd = "waiting 1 second.";
	addLog();
	Sleep(1000);
}

void wait5()
{
	textToAdd = "waiting 5 seconds.";
	addLog();
	Sleep(5000);
}

void testservo(int daisychain, int servonumber)
{
	I2C_MoveServo(S1, daisychain, servonumber, 1);
	Sleep(100);
	I2C_MoveServo(S1, daisychain, servonumber, 255);
	Sleep(100);
	I2C_MoveServo(S1, daisychain, servonumber, 1);
	Sleep(100);
	I2C_MoveServo(S1, daisychain, servonumber, 268);
}

task main()
{
	textToAdd = "system test initialized.";
	addLog();
	textToAdd = "using I2C functions.";
	addLog();

	textToAdd = "waiting 5 seconds for the arm to reset...";
	addLog();
	for (int i = 0; i < 5000; i++)
	{
		Arm_Update();
		Sleep(1);
	}

	textToAdd = "moving the arm to a sane place...";
	addLog();
	Arm_SetSpeed(15);
	for (int i = 0; i < 30; i++)
	{
		Arm_Update();
		Sleep(1);
	}

	textToAdd = "testing encoders...";
	addLog();
	textToAdd = "using daisychain 1, motor 1.";
	addLog();
	testEncoder(1, 1);
	textToAdd = "using daisychain 1, motor 2.";
	addLog();
	testEncoder(1, 2);
	textToAdd = "using daisychain 2, motor 1.";
	addLog();
	testEncoder(2, 1);
	textToAdd = "using daisychain 2, motor 2.";
	addLog();
	testEncoder(2, 2);

	wait5();

	textToAdd = "testing motors...";
	addLog();
	textToAdd = "using daisychain 1, motor 1.";
	addLog();
	textToAdd = "running at power level 100.";
	addLog();
	testMotor(1, 1);
	textToAdd = "using daisychain 1, motor 1.";
	addLog();
	textToAdd = "running at power level 100.";
	addLog();
	testMotor(1, 2);
	textToAdd = "using daisychain 1, motor 1.";
	addLog();
	textToAdd = "running at power level 100.";
	addLog();
	testMotor(2, 1);
	textToAdd = "using daisychain 2, motor 2.";
	addLog();
	textToAdd = "running at power level 100.";
	addLog();
	testMotor(2, 2);

	wait5();

	textToAdd = "testing servos...";
	addLog();
	textToAdd = "using daisychain 3, servo 1.";
	addLog();
	testservo(3, 1);

	textToAdd = "physical systems check complete.";
	addLog();
	textToAdd = "software systems check unimplemented.";
	addLog();
	textToAdd = "systems check complete.";
	addLog();
}
