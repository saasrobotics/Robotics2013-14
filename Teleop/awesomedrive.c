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

#include "JoystickDriver.c"
#include "../libraries/I2C.h"

task main()
{

	while (true)
	{
		// read the joystick
		getJoystickSettings(joystick);

		// calculate the drive speed
		int driveSpeedBase = defaultDriveSpeed + (joystick.joy1_x2*2);

		// calculate the two sides' drive
		int driveSpeedRight = driveSpeedBase-joystick.joy1_y1;
		int driveSpeedLeft = driveSpeedBase+joystick.joy1_y1;

		I2C_SetMotorSpeed(S1, 2, 1, driveSpeedRight);
		I2C_SetMotorSpeed(S1, 2, 2, driveSpeedLeft);

		Sleep(10);
	}

}
