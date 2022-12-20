/*
Created by: Arjun Mandair, Kevin Shu, Abeed Chowdhury, Shiv Patel
Created on: November 14, 2022
Last modified: November 25, 2022
Version: 1.0

Purpose: To articulate a solar panel to make it orthogonal to the
sunlight

Assumptions: We are assuming the wires dont get caught, and that there
is no human interaction while the robot is on or off
*/

//Function Prototypes
void configSensors();	//Configures functions
void startup();	//Runs startup procedure
int lightCalc();	//Calculates light intensity levels
bool yawDir(const int & lightInt);	//Determines yaw direction
void articulate(const int & lightInt, const bool & clockwise);	//Articulates
void shutdown();	//Runs shutdown Procedure
int timer(); //Runs timer between articulation intervals

//Declaring constant variables
const int TWO_MIN = 5000;
const int TOLERANCE = 10;
const int TEN_SEC = 10000;

//Main code
task main()
{
	//Initializing variables and nmotorencoders
	nMotorEncoder[motorD] = 0;
	nMotorEncoder[motorB] = 0;
	nMotorEncoder[motorC] = 0;
	int lightInt = 0;
	bool clockwise = false;

	//Configuring sensors
	configSensors();

	//Waiting for enter button press
	while(!getButtonPress(buttonEnter))
	{}
	while(getButtonPress(buttonEnter))
	{}

	//Initiating startup procedure
	startup();

	//Determining if program should run
	lightInt = lightCalc();

	//Program runs while lightInt !=-1
	while(lightInt != -1)
	{
		//Determining yaw direction
		clockwise = yawDir(lightInt);

		//Articulating solar panel
		articulate(lightInt, clockwise);

		//Waiting two minute and then checking average light intensity again
		lightInt = timer();

		//Checking if enter button is pressed
		if(lightInt != -1)
		{
			lightInt = lightCalc();
		}
	}

	shutdown();
	wait1Msec(TEN_SEC);
}


//Function that configures color sensors to read light levels
void configSensors()
{
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(100);
	SensorMode[S1] = modeEV3Color_Ambient;
	wait1Msec(100);
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(100);
	SensorMode[S2] = modeEV3Color_Ambient;
	wait1Msec(100);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(100);
	SensorMode[S3] = modeEV3Color_Ambient;
	wait1Msec(100);
	SensorType[S4] = sensorEV3_Color;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Color_Ambient;
	wait1Msec(100);

	return;
}


//Function that runs startup procedure
void startup()
{
	//Removing cover
	motor[motorB] = -20;
	wait1Msec(800);
	motor[motorB] = 0;

	//Rotating yaw cw
	nMotorEncoder[motorC] = 0;
	motor[motorC] = 25;
	while(abs(nMotorEncoder[motorC]) < 90)
	{}
	motor[motorC] = 0;
	wait1Msec(1000);

	//Rotating yaw ccw
	nMotorEncoder[motorC] = 0;
	motor[motorC] = -25;
	while(abs(nMotorEncoder[motorC]) < 180)
	{}
	motor[motorC] = 0;
	wait1Msec(1000);

	//Rotating yaw cw back to original position
	nMotorEncoder[motorC] = 0;
	motor[motorC] = 25;
	while(abs(nMotorEncoder[motorC]) < 90)
	{}
	motor[motorC] = 0;
	wait1Msec(1000);
	nMotorEncoder[motorC] = 0;

	//Rotating pitch ccw
	nMotorEncoder[motorD] = 0;
	motor[motorD] = 10;
	while(abs(nMotorEncoder[motorD]) < 90)
	{}
	motor[motorD] = 0;
	wait1Msec(500);

	//Rotating pitch cw
	nMotorEncoder[motorD] = 0;
	motor[motorD] = -10;
	while(abs(nMotorEncoder[motorD]) < 180)
	{}
	motor[motorD] = 0;
	wait1Msec(500);

	//Returning pitch to original location
	nMotorEncoder[motorD] = 0;
	motor[motorD] = 10;
	while(abs(nMotorEncoder[motorD]) < 90)
	{}
	motor[motorD] = 0;
	wait1Msec(500);
	nMotorEncoder[motorD] = 0;

	return;
}


//Function that calulates the average light intensity level
int lightCalc()
{
	//Reading color sensor values
	int colorOne = SensorValue[S1];
	int colorTwo = SensorValue[S2];
	int colorThree = SensorValue[S3];
	int colorFour = SensorValue[S4];

	//Reading average light intensity
	int avgLightInt = (colorOne + colorTwo + colorThree + colorFour) / 4;

	//Diplaying values on EV3 screen
	displayString(1, "colorOne: %d", SensorValue[S1]);
	displayString(2, "colorTwo: %d", SensorValue[S2]);
	displayString(3, "colorThree: %d", SensorValue[S3]);
	displayString(4, "colorFour: %d", SensorValue[S4]);
	displayString(5, "Avg lightInt: %d", avgLightInt);

	//Activating shutdown procedure
	if (avgLightInt == 0)
	{
		displayString(7, "Average light intensity is 0");
		displayString(8, " ");
		return -1;
	}

	//Color sensor 1 is the highest and displaying
	if(colorOne > colorTwo && colorOne > colorThree && colorOne > colorFour)
	{
		displayString(7, "Highest sensor value is S1");
		displayString(8, "with sensor value of %d", SensorValue[S1]);
		return 1;
	}

	//Color sensor 2 is the highest and displaying
	if(colorTwo > colorOne && colorTwo > colorThree && colorTwo > colorFour)
	{
		displayString(7, "Highest sensor value is S2");
		displayString(8, "with sensor value of %d", SensorValue[S2]);
		return 2;
	}

	//Color sensor 3 is the highest and displaying
	if(colorThree > colorOne && colorThree > colorTwo && colorThree > colorFour)
	{
		displayString(7, "Highest sensor value is S3");
		displayString(8, "with sensor value of %d", SensorValue[S3]);
		return 3;
	}

	//Color sensor 4 is the highest and displaying
	displayString(7, "Highest sensor value is S4");
	displayString(8, "with sensor value of %d", SensorValue[S4]);
	return 4;
}


//Function that determines yaw direction
bool yawDir(const int & lightInt)
{
	if(lightInt == 2 || lightInt == 4)	//Robot will turn ccw
	{
		return false;
	}
	else	//Robot will turn cw
	{
		return true;
	}
}


//Function that articulates the solar panel
void articulate(const int & lightInt, const bool & clockwise)
{
	//Rotating yaw
	if(clockwise == true)	//Rotating yaw clockwise
	{
		motor[motorC] = -10;
		if(lightInt == 1) //Comparing S1 and S2 while turning cw
		{
			//Waiting for colorOne to equal colorTwo
			while(!(SensorValue[S1] <= (SensorValue[S2] + TOLERANCE)
				&& SensorValue[S1] >= (SensorValue[S2] - TOLERANCE)))
			{}
		}
		else //Comparing S3 and S4 while turning cw
		{
			//Waiting for colorOne to equal colorTwo
			while(!(SensorValue[S3] <= (SensorValue[S4] + TOLERANCE)
				&& SensorValue[S3] >= (SensorValue[S4] - TOLERANCE)))
			{}
		}
	}
	else	//Rotating yaw counterclockwise
	{
		motor[motorC] = 10;
		if(lightInt == 2) //Comparing S1 and S2 while turning ccw
		{
			//Waiting for colorThree to equal coorFour
			while(!(SensorValue[S1] <= (SensorValue[S2] + TOLERANCE)
				&& SensorValue[S1] >= (SensorValue[S2] - TOLERANCE)))
			{}
		}
		else //Comparing S3 and S4 while turning ccw
		{
			//Waiting for colorThree to equal coorFour
			while(!(SensorValue[S3] <= (SensorValue[S4] + TOLERANCE)
				&& SensorValue[S3] >= (SensorValue[S4] - TOLERANCE)))
			{}
		}
	}

	motor[motorC] = 0;

	//Rotating pitch
	if(SensorValue[S1] > SensorValue[S3])	//Rotating pitch clockwise
	{
		motor[motorD] = 10;
	}
	else if(SensorValue[S1] < SensorValue[S3]) //Rotating pitch counter-clockwise
	{
		motor[motorD] = -10;
	}

	//Waiting for all four colour sensors to be equal
	while(!(SensorValue[S1] <= (SensorValue[S3] + TOLERANCE)
		&& SensorValue[S1] >= (SensorValue[S3] - TOLERANCE)))
	{}

	//Shuttig off motorD
	motor[motorD] = 0;

	//Diplaying values on EV3 screen
	displayString(10, "colorOne: %d", SensorValue[S1]);
	displayString(11, "colorTwo: %d", SensorValue[S2]);
	displayString(12, "colorThree: %d", SensorValue[S3]);
	displayString(13, "colorFour: %d", SensorValue[S4]);

	return;
}


//Function that runs shutdown procedure
void shutdown()
{
	//Returning platform to original position
	if(nMotorEncoder[motorD] > 0)
	{
		motor[motorD] = -5;
	}
	else if(nMotorEncoder[motorD] < 0)
	{
		motor[motorD] = 5;
	}
	else
	{}

	while(!(abs(nMotorEncoder[motorD]) <= 1 && abs(nMotorEncoder[motorD]) >= -1))
	{}

	motor[motorD] = 0;

	//Deploying cover
	motor[motorB] = 20;
	wait1Msec(800);
	motor[motorB] = 0;

	//Returning yaw to original position
	if(nMotorEncoder[motorC] > 0)
	{
		motor[motorC] = -10;
	}
	else
	{
		motor[motorC] = 10;
	}

	while(!(abs(nMotorEncoder[motorC]) <= 1 && abs(nMotorEncoder[motorC]) >= -1))
	{}
	motor[motorC] = 0;
}

//Function that waits two min between articulation,
//returns -1 if enter button has been press
int timer()
{
	time1[T1] = 0;

	//Checking if enter button has been pressed
	while(time1[T1] < TWO_MIN)
	{
		if(getButtonPress(buttonEnter))
		{
			while(getButtonPress(buttonEnter))
			{}
			return -1;
		}
	}

	//Returning platform to original position
	if(nMotorEncoder[motorD] > 0)
	{
		motor[motorD] = -5;
	}
	else if(nMotorEncoder[motorD] < 0)
	{
		motor[motorD] = 5;
	}
	else
	{}

	while(!(abs(nMotorEncoder[motorD]) <= 1 && abs(nMotorEncoder[motorD]) >= -1))
	{}

	motor[motorD] = 0;
	nMotorEncoder[motorD] = 0;
	return 0;
}
