
			#pragma config(Sensor, S1,     M_CONTROL,      sensorI2CCustom)
			#pragma config(Sensor, S2,     M_CONTROL,      sensorI2CCustom)
			#pragma config(Sensor, S3,     M_CONTROL,      sensorI2CCustom)
			#pragma config(Sensor, S4,     M_CONTROL,      sensorI2CCustom)

			#pragma systemFile

			#ifndef __COMMON_H__
			#define __COMMON_H__

			#undef __COMMON_H_DEBUG__
			//#define __COMMON_H_DEBUG__

			/*!< define this as 0 to remove the check  */
			#ifndef __COMMON_H_SENSOR_CHECK__
			#define __COMMON_H_SENSOR_CHECK__ 1
			#else
			#warn "sensor checking disabled, I hope you know what you are doing!"
			#endif

			#include "firmwareVersion.h"
			#if (kRobotCVersionNumeric < 351)
			#error "These drivers are only supported on RobotC version 3.51 or higher"
			#endif

			#ifndef MAX_ARR_SIZE
			/**
			 * Maximum buffer size for byte_array, can be overridden in your own program.
			 * It's 17 bytes big because the max I2C buffer size is 16, plus 1 byte to denote
			 * packet length.
			 */
			#define MAX_ARR_SIZE 17
			#endif


			/**
			 * This define returns the smaller of the two numbers
			 */
			#define min2(a, b) (a < b ? a : b)


			/**
			 * This define returns the smallest of the three numbers
			 */
			#define min3(a, b, c) (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c)


			/**
			 * This function returns the bigger of the two numbers
			 */
			#define max2(a, b) (a > b ? a : b)


			/**
			 * This function returns the biggest of the three numbers
			 */
			#define max3(a, b, c) (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c)


			/**
			 * Returns x if it is between min and max. If outside the range,
			 * it returns min or max.
			 */
			#define clip(a, b, c) min2(c, max2(b, a))


			/**
			 * Array of bytes as a struct, this is a work around for RobotC's inability to pass an array to
			 * a function.
			 */
			typedef ubyte tByteArray[MAX_ARR_SIZE];
			typedef sbyte tsByteArray[MAX_ARR_SIZE];
			typedef ubyte tMassiveArray[128];             /*!< 128 byte array for very large blobs of data */
			typedef ubyte tHugeByteArray[64];             /*!< 64 byte array for very large blobs of data */
			typedef ubyte tBigByteArray[32];              /*!< 32 byte array for large blobs of data */
			typedef ubyte tIPaddr[4];                     /*!< Struct for holding an IP address */

			/**
			 * Array of ints as a struct, this is a work around for RobotC's inability to pass an array to
			 * a function.
			 */
			typedef int tIntArray[MAX_ARR_SIZE];

			void clearI2CError(tSensors link, ubyte address);
			bool waitForI2CBus(tSensors link);
			bool writeI2C(tSensors link, tByteArray &request, tByteArray &reply, int replylen);
			bool writeI2C(tSensors link, tByteArray &request);


			/**
			 * Clear out the error state on I2C bus by sending a bunch of dummy
			 * packets.
			 * @param link the port number
			 * @param address the I2C address we're sending to
			 */
			void clearI2CError(tSensors link, ubyte address) {
			  ubyte error_array[2];
			  error_array[0] = 1;           // Message size
			  error_array[1] = address; // I2C Address

			#ifdef __COMMON_H_DEBUG__
			  eraseDisplay();
			  nxtDisplayTextLine(3, "rxmit: %d", ubyteToInt(error_array[1]));
			  wait1Msec(2000);
			#endif // __COMMON_H_DEBUG__

			  for (int i = 0; i < 5; i++) {
				sendI2CMsg(link, &error_array[0], 0);
				wait1Msec(5);
			  }
			}


			/**
			 * Wait for the I2C bus to be ready for the next message
			 * @param link the port number
			 * @return true if no error occured, false if it did
			 */
			bool waitForI2CBus(tSensors link)
			{
			  //TI2CStatus i2cstatus;
			  while (true)
			  {
				//i2cstatus = nI2CStatus[link];
				switch (nI2CStatus[link])
				//switch(i2cstatus)
				{
					case NO_ERR:
					  return true;

					case STAT_COMM_PENDING:
					  break;

					case ERR_COMM_CHAN_NOT_READY:
					  break;

					case ERR_COMM_BUS_ERR:
				#ifdef __COMMON_H_DEBUG__
					  PlaySound(soundLowBuzz);
					  while (bSoundActive) {}
				#endif // __COMMON_H_DEBUG__
					return false;
				}
				EndTimeSlice();
			  }
			}


			/**
			 * Write to the I2C bus. This function will clear the bus and wait for it be ready
			 * before any bytes are sent.
			 * @param link the port number
			 * @param request the data to be sent
			 * @return true if no error occured, false if it did
			 */
			bool writeI2C(tSensors link, tByteArray &request) {

			#if (__COMMON_H_SENSOR_CHECK__ == 1)
			  //TSensorTypes type = SensorType[link];

			  switch (SensorType[link])
			  {
				case sensorI2CCustom:                 break;
				case sensorI2CCustom9V:               break;
				case sensorI2CCustomFast:             break;
				case sensorI2CCustomFast9V:           break;
				case sensorI2CCustomFastSkipStates9V: break;
				case sensorI2CCustomFastSkipStates:   break;
				default:
					hogCPU();
					PlaySound(soundException);
					eraseDisplay();
					nxtDisplayCenteredTextLine(0, "3rd Party Driver");
					nxtDisplayCenteredTextLine(1, "ERROR");
					nxtDisplayCenteredTextLine(2, "You have not");
					nxtDisplayCenteredTextLine(3, "setup the sensor");
					nxtDisplayCenteredTextLine(4, "port correctly. ");
					nxtDisplayCenteredTextLine(5, "Please refer to");
					nxtDisplayCenteredTextLine(6, "one of the");
					nxtDisplayCenteredTextLine(7, "examples.");
					wait1Msec(10000);
					StopAllTasks();
			  }
			#endif

			  if (!waitForI2CBus(link)) {
				clearI2CError(link, request[1]);

				// Let's try the bus again, see if the above packets flushed it out
				// clearI2CBus(link);
				if (!waitForI2CBus(link))
				  return false;
			  }

			  sendI2CMsg(link, &request[0], 0);

			  if (!waitForI2CBus(link)) {
				clearI2CError(link, request[1]);
				sendI2CMsg(link, &request[0], 0);
				if (!waitForI2CBus(link))
				  return false;
			  }
			  return true;
			}


			/**
			 * Write to the I2C bus. This function will clear the bus and wait for it be ready
			 * before any bytes are sent.
			 * @param link the port number
			 * @param request the data to be sent
			 * @param reply array to hold received data
			 * @param replylen the number of bytes (if any) expected in reply to this command
			 * @return true if no error occured, false if it did
			 */
			bool writeI2C(tSensors link, tByteArray &request, tByteArray &reply, int replylen) {
			  // clear the input data buffer

			#if (__COMMON_H_SENSOR_CHECK__ == 1)
			  //TSensorTypes type = SensorType[link];

			  switch (SensorType[link])
			  {
				case sensorI2CCustom:                 break;
				case sensorI2CCustom9V:               break;
				case sensorI2CCustomFast:             break;
				case sensorI2CCustomFast9V:           break;
				case sensorI2CCustomFastSkipStates9V: break;
				case sensorI2CCustomFastSkipStates:   break;
				default:
					hogCPU();
					PlaySound(soundException);
					eraseDisplay();
					nxtDisplayCenteredTextLine(0, "3rd Party Driver");
					nxtDisplayCenteredTextLine(1, "ERROR");
					nxtDisplayCenteredTextLine(2, "You have not");
					nxtDisplayCenteredTextLine(3, "setup the sensor");
					nxtDisplayCenteredTextLine(4, "port correctly. ");
					nxtDisplayCenteredTextLine(5, "Please refer to");
					nxtDisplayCenteredTextLine(6, "one of the");
					nxtDisplayCenteredTextLine(7, "examples.");
					wait1Msec(10000);
					StopAllTasks();
			  }
			#endif

			  if (!waitForI2CBus(link)) {
				clearI2CError(link, request[1]);

				// Let's try the bus again, see if the above packets flushed it out
				// clearI2CBus(link);
				if (!waitForI2CBus(link))
				  return false;
			  }

			  sendI2CMsg(link, &request[0], replylen);

			  if (!waitForI2CBus(link)) {
				clearI2CError(link, request[1]);
				sendI2CMsg(link, &request[0], replylen);
				if (!waitForI2CBus(link))
				  return false;
			  }

			  // ask for the input to put into the data array
			  readI2CReply(link, &reply[0], replylen);

			  return true;
			}

			/**
			 * Create a unique ID (UID) for an NXT.  This based on the last 3 bytes
			 * of the Bluetooth address.  The first 3 bytes are manufacturer
			 * specific and identical for all NXTs and are therefore not used.
			 * @return a unique ID for the NXT.
			 */
			long getUID() {
			  TBTAddress btAddr;
			  getBTAddress(btAddr);

			  // Only last 3 bytes are unique in the BT address, the other three are for the
			  // manufacturer (LEGO):  http://www.coffer.com/mac_find/?string=lego
			   return (long)btAddr[5] + ((long)btAddr[4] << 8) + ((long)btAddr[3] << 16);
			}


			#endif // __COMMON_H__

void I2C_WritePMW(tSensors port, int DaisyChainLevel, byte Status)
{
        tByteArray I2Crequest;
        tByteArray I2Cresponse;
        I2Crequest[0] = 3;
        I2Crequest[1] = 0x02 * DaisyChainLevel;
        I2Crequest[2] = 0x48;
        I2Crequest[3] = Status;
        writeI2C(port, I2Crequest, I2Cresponse, 1);
}

void I2C_MoveServo(tSensors port, int DaisyChainLevel, int ServoNumber, byte Position)
{
	tByteArray I2Crequest;
	I2Crequest[0] = 3;
	I2Crequest[1] = 0x02 * DaisyChainLevel;
	I2Crequest[2] = 0x41 + ServoNumber;
	I2Crequest[3] = Position;

	writeI2C(port, I2Crequest);
}

void I2C_SetMode(tSensors port, int DaisyChainLevel, int MotorNumber, byte Mode)
{
	if(MotorNumber == 1)
	{
		tByteArray I2Crequest;
		I2Crequest[0] = 3;
		I2Crequest[1] = 0x02 * DaisyChainLevel;
		I2Crequest[2] = 0x44;
		I2Crequest[3] = Mode;
		writeI2C(port, I2Crequest);
	}
	else if (MotorNumber == 2)
	{
		tByteArray I2Crequest;
		I2Crequest[0] = 3;
		I2Crequest[1] = 0x02 * DaisyChainLevel;
		I2Crequest[2] = 0x47;
		I2Crequest[3] = Mode;
		writeI2C(port, I2Crequest);
	}
}

void I2C_SetMotorSpeed(tSensors port, int daisychainLevel, int MotorNumber, sbyte Speed)
{
	tByteArray I2Crequest;

	I2Crequest[0] = 3;

	// daisychain level 0 will add 0, daisychain level 1 will add 2 to get 0x04, etc.
	I2Crequest[1] = 0x02 * daisychainLevel;

	if(MotorNumber == 1)
	{
		I2Crequest[2] = 0x45;
		//if we're on motor 1 mode comes first...
		//I2Crequest[3] = 0b00010001;
		I2Crequest[3] = Speed;
	}
	else
	{
		I2Crequest[2] = 0x46;
		// ...but if we're on motor 2, speed comes first.
		I2Crequest[3] = Speed;
		//I2Crequest[4] = 0b00010001;
	}

	writeI2C(port, I2Crequest);
}

// pass this 1 or 2 for the motor and S[1-4] for the port
long I2C_GetEncoderPosition(tSensors port, int daisychainLevel, int MotorNumber)
{
		daisychainLevel--;
		//initializes the arrays
		tByteArray I2Crequest;
		tByteArray I2Cresponse;

		//sets the number of bytes to send:
		I2Crequest[0] = 2;

		//sends the adress as the first byte
		//daisychain level 0 will add 0, daisychain level 1 will add 2 to get 0x04, etc.
	  I2Crequest[1] = 0x02 + daisychainLevel*2;

	  //sets the starting position to start sending data at
	  if (MotorNumber == 1)
	  {
	  	I2Crequest[2] = 0x4C;
		}
		else
		{
		  I2Crequest[2] = 0x50;
		}

	  //writes the data, and gets the response
	  writeI2C(port, I2Crequest, I2Cresponse, 4);
		//writeDebugStreamLine("%i", I2Cresponse[1]);
	  //creates a long out of the bytes
	  //note: when debugging with any %i construct, this will be cast to an integer and will overflow at 32767!
	  long EncoderValue = (I2Cresponse[0] << 24) + (I2Cresponse[1] << 16) + (I2Cresponse[2] << 8) + (I2Cresponse[3] << 0);

	  //returns the long
	  return EncoderValue;
  	//return ConvertBytesToLong(I2Cresponse[0], I2Cresponse[1], I2Cresponse[2], I2Cresponse[3]);
}

// motor should be 1 or 2, port should be S[1-4], Input should be the position to move to
void I2C_SetEncoderPosition(tSensors port, int daisychainLevel, int MotorNumber, long EncoderPosition, byte MotorSpeed)
{
  if (MotorNumber == 1)
  {
  	tByteArray I2Crequest;
  	I2Crequest[0] = 6;
  	I2Crequest[1] = 0x02 * daisychainLevel;
  	I2Crequest[2] = 0x40;
  	I2Crequest[3] = (byte)((EncoderPosition >> 24) & 0x000000ff);
	  I2Crequest[4] = (byte)((EncoderPosition >> 16) & 0x000000ff);
	  I2Crequest[5] = (byte)((EncoderPosition >> 8) & 0x000000ff);
	  I2Crequest[6] = (byte)(EncoderPosition & 0x000000ff);
		writeI2C(port, I2Crequest);

		tByteArray I2Crequest2;
		I2Crequest2[0] = 3;
		I2Crequest2[1] = 0x02 * daisychainLevel;
		I2Crequest2[2] = 0x45;
	  I2Crequest2[3] = MotorSpeed;
	  writeI2C(port, I2Crequest2);
	}
	else if (MotorNumber == 2)
	{
		tByteArray I2Crequest;
		I2Crequest[0] = 3;
		I2Crequest[1] = 0x02 * daisychainLevel;
  	I2Crequest[2] = 0x46;
	  I2Crequest[3] = MotorSpeed;
	  writeI2C(port, I2Crequest);

	  tByteArray I2Crequest2;
	  I2Crequest2[0] = 6;
	  I2Crequest2[1] = 0x02 * daisychainLevel;
	  I2Crequest2[2] = 0x48;
	  I2Crequest2[3] = (byte)((EncoderPosition >> 24) & 0x000000ff);
	  I2Crequest2[4] = (byte)((EncoderPosition >> 16) & 0x000000ff);
	  I2Crequest2[5] = (byte)((EncoderPosition >> 8) & 0x000000ff);
	  I2Crequest2[6] = (byte)(EncoderPosition & 0x000000ff);
	  writeI2C(port, I2Crequest2);
  }
}
void Servos_SetPosition(tSensors Port, int DaisyChainLevel, int MotorNumber, int Position)
{
	I2C_WritePMW(Port, DaisyChainLevel, 0);
	I2C_MoveServo(Port, DaisyChainLevel, MotorNumber, Position);
}

int Motors_Length = 0;

tSensors Motors_Port[10];
int Motors_DaisyChainLevel[10];
int Motors_MotorNumber[10];
byte Motors_Mode[10];


byte Motors_ModeByLocation(tSensors Port, int DaisyChainLevel, int MotorNumber)
{
	for(int i = 0; i < Motors_Length; i++)
	{
		if(Port == Motors_Port[i] && DaisyChainLevel == Motors_DaisyChainLevel[i] && MotorNumber == Motors_MotorNumber[i])
		{
			return Motors_Mode[i];
		}
	}
	return 0b00000000;
}

int Motors_ContainsMode(tSensors Port, int DaisyChainLevel, int MotorNumber)
{
	for(int i = 0; i < Motors_Length; i++)
	{
		if(Port == Motors_Port[i] && DaisyChainLevel == Motors_DaisyChainLevel[i] && MotorNumber == Motors_MotorNumber[i])
		{
			return i;
		}
	}
	return -1;
}

void Motors_NewMode(tSensors Port, int DaisyChainLevel, int MotorNumber, byte Mode)
{
	I2C_SetMode(Port, DaisyChainLevel, MotorNumber, Mode);

	int IndexOfList = Motors_ContainsMode(Port, DaisyChainLevel, MotorNumber);
	if(IndexOfList != -1)
	{
		Motors_Mode[IndexOfList] = Mode;
	}
	else
	{
		Motors_DaisyChainLevel[Motors_Length] = DaisyChainLevel;
		Motors_MotorNumber[Motors_Length] = MotorNumber;
		Motors_Port[Motors_Length] = Port;
		Motors_Mode[Motors_Length] = Mode;
		Motors_Length++;
	}
}

void Motors_SetSpeed(tSensors Port, int DaisyChainLevel, int MotorNumber, int Speed)
{
	if(MotorNumber == 1)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, 0b00010001);
		Motors_NewMode(Port, DaisyChainLevel, 2, Motors_ModeByLocation(Port, DaisyChainLevel, 2) | 0b00010000);

		I2C_SetMotorSpeed(Port, DaisyChainLevel, MotorNumber, Speed);
	}
	else if (MotorNumber == 2)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, Motors_ModeByLocation(Port, DaisyChainLevel, 1) | 0b00010000);
		Motors_NewMode(Port, DaisyChainLevel, 2, 0b00010001);

		I2C_SetMotorSpeed(Port, DaisyChainLevel, MotorNumber, Speed);
	}
}

void Motors_SetPosition(tSensors Port, int DaisyChainLevel, int MotorNumber, long MotorEncoder, int Speed)
{
	if(MotorNumber == 1)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, 0b00010010);
		Motors_NewMode(Port, DaisyChainLevel, 2, Motors_ModeByLocation(Port, DaisyChainLevel, 2) | 0b00010000);

		I2C_SetEncoderPosition(Port, DaisyChainLevel, MotorNumber, MotorEncoder, Speed);
	}
	else if (MotorNumber == 2)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, Motors_ModeByLocation(Port, DaisyChainLevel, 1) | 0b00010000);
		Motors_NewMode(Port, DaisyChainLevel, 2, 0b00010010);

		I2C_SetEncoderPosition(Port, DaisyChainLevel, MotorNumber, MotorEncoder, Speed);
	}
}

void Motors_MoveRotations(tSensors Port, int DaisyChainLevel, int MotorNumber, float Rotations, int Speed)
{

	Motors_SetPosition(Port, DaisyChainLevel, MotorNumber, I2C_GetEncoderPosition(Port, DaisyChainLevel, MotorNumber) + (Rotations * 1440), Speed);
}

		task main() {
Motors_SetPosition(S1, 2, 1, (I2C_GetEncoderPosition(S1, 2, 1) + 7200), 10);
Motors_SetPosition(S1, 2, 2, (I2C_GetEncoderPosition(S1, 2, 2) + -7200), 10);
Sleep(10000);
Motors_SetSpeed(S1, 2, 1, 0);
Motors_SetSpeed(S1, 2, 2, 0);
}