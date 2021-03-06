#pragma config(Sensor, S1,     ,               sensorI2CCustom)
#pragma config(Sensor, S2,     ,               sensorI2CCustom)
#pragma config(Sensor, S3,     ,               sensorI2CCustom)
#pragma config(Sensor, S4,     ,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*

	Awesomedrive Teliop:
		Right joy controls speed.
		Left joy controls direction. Forward and backward is atomic; left and right is incremental.

	Motors:
		Right side is daisychain 2 motor 1, looking at the robot from the top in the upwards "U" orientation
		Left side is daisychain 2 motor 2

*/

// program parameters
const int defaultDriveSpeed = 25;
const int threshold = 8;

#include "JoystickDriver.c"
#include "../libraries/I2C.h"

task main()
{

	while (true)
	{
		// read the joystick
		getJoystickSettings(joystick);

		// calculate the drive speed
		int driveSpeedBase = defaultDriveSpeed + (joystick.joy1_y2/3);

		// don't calculate a negative drive speed
		if (driveSpeedBase < 5) driveSpeedBase = 5;

		// decide if we're going to be driving forward at all
		if (abs(joystick.joy1_y1) < threshold)
		{
			driveSpeedBase = 0;
		}

		// decide if we're going to be driving backwards
		if (joystick.joy1_y1 > 0)
		{
			driveSpeedBase = -driveSpeedBase;
		}

		// right and left drive speeds inherit from the base drive speed
		int driveSpeedRight, driveSpeedLeft = driveSpeedBase;

		// calculate the two sides' drive
		//driveSpeedRight = driveSpeedRight-(joystick.joy1_x1/3);
		//driveSpeedLeft = driveSpeedLeft+(joystick.joy1_x1/3);

		// account for the right motor set being the inverse
		driveSpeedRight = -driveSpeedRight;

		I2C_SetMotorSpeed(S1, 2, 1, driveSpeedRight);
		I2C_SetMotorSpeed(S1, 2, 2, driveSpeedLeft);

		Sleep(10);
	}

}
