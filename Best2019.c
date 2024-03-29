#pragma config(Sensor, dgtl2,  irLeft,         sensorDigitalIn)
#pragma config(Sensor, dgtl3,  irRight,        sensorDigitalIn)
#pragma config(Sensor, dgtl6,  limitSwitch,    sensorTouch)
#pragma config(Motor,  port2,           motorLeft,     tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           conduitServo1, tmotorServoStandard, openLoop)
#pragma config(Motor,  port4,           motorRight,    tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port5,           autoRelease,   tmotorServoStandard, openLoop)
#pragma config(Motor,  port7,           conduitServo2, tmotorServoStandard, openLoop)
#pragma config(Motor,  port8,           motorRP,       tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port9,           sliderServo,   tmotorServoStandard, openLoop)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Global Variables
float gear = 1;
int direction = true;
int prev7R = false;
int prev7U = false;
int prev7D = false;
int prev6U = false;
int prev7L = false;
int prev8R = false;
int servoPos = 128;
int servoAutoClose = 2000; //Milliseconds
bool sliderDown = true;
int getIrReading(tSensors irReceiverPin)
{
	wait1Msec(1);
	int ir = SensorValue[irReceiverPin];
	wait1Msec(1);
	return ir;
}


task main()
{
	//Setup Stuff
	motor[conduitServo1] = 128; //Set servo to correct start position
	motor[conduitServo2] = 128; //Set servo to correct start position
	motor[autoRelease] = 128;
	while (true)
	{

		//Rack and Pinion Code
		if(vexRT[Btn5U] == 1)
			motor[motorRP] = -100;//Up Speed
		else if(vexRT[Btn5D] == 1)
			motor[motorRP] = 75;//Down Speed
		else
			motor[motorRP] = -25;//Default (Brake)


		// Servo Code
		if (vexRT[Btn7U] != prev7U || vexRT[Btn6U] != prev6U){//Detect button change
			prev7U = vexRT[Btn7U];
			prev6U = vexRT[Btn6U];
			if(vexRT[Btn7U] == 1 || vexRT[Btn6U] == 1)//Only run on rising edge (aka press, not release)
				servoPos = (servoPos == 128) ? -50 : 128;
			motor[conduitServo1] = servoPos;
			motor[conduitServo2] = servoPos;
			clearTimer(timer1);
		}

		//Automatically close hatch after too long
		if(time1[timer1] >= servoAutoClose){
			servoPos = 128;
			motor[conduitServo1] = servoPos;
			motor[conduitServo2] = servoPos;
		}

		// Slider Servo Code
		//if () {
			if (vexRT[Btn7D] != prev7D) {
				prev7D = vexRT[Btn7D];
				if(vexRT[Btn7D] == 1)
					sliderDown = !sliderDown;
				motor[sliderServo] = (sliderDown) ? -128: 128;
			}

	//	}

		// GearBox

		if (vexRT[Btn8R] != prev8R){
			prev8R = vexRT[Btn8R];
			if(vexRT[Btn8R] == 1)
				gear = (gear == 1) ? 0.5 : 1;
		}

		// Direction

		if (vexRT[Btn7R] != prev7R){
			prev7R = vexRT[Btn7R];
			if(vexRT[Btn7R] == 1)
				direction = (direction == 1) ? -1 : 1;
		}
/*
		if (vexRT[Btn7D] != prev7D){
			prev7D = vexRT[Btn7D];
			if(vexRT[Btn7D] == 1)
				direction = (direction == 1) ? -1 : 1;
		}
*/

		// Drive
		if(2==1){
			motor[motorLeft] = -15;
			motor[motorRight] = 15;
		}
		else{
			if(direction == 1){
		    if(vexRT[Ch3]<20 && vexRT[Ch3]>-20 && vexRT[Btn6D]==1){
		    	motor[motorLeft] = -20;
		    }
				else if (vexRT[Ch3] > 0)
					motor[motorLeft] = gear * 100 * (1 / (1 + pow(2.718, -((vexRT[Ch3]/8) - 6))));
				else if (vexRT[Ch3] < 0)
					motor[motorLeft] = -gear * 100 * (1 / (1 + pow(2.718, -((-vexRT[Ch3]/8) - 6))));
				else
					motor[motorLeft] = 0;

			  if(vexRT[Ch2]<20 && vexRT[Ch2]>-20 && vexRT[Btn6D]==1){
		    	motor[motorRight] = 20;
		    }
				else if (vexRT[Ch2] > 0)
					motor[motorRight] = -gear * 100 * (1 / (1 + pow(2.718, -((vexRT[Ch2]/8) - 6))));
				else if (vexRT[Ch2] < 0)
					motor[motorRight] = gear * 100 * (1 / (1 + pow(2.718, -((-vexRT[Ch2]/8) - 6))));
				else
					motor[motorRight] = 0;
			}
			else{
				if(vexRT[Ch3]<20 && vexRT[Ch3]>-20 && vexRT[Btn6D]==1){
		    	motor[motorRight] = 20;
		    }
				else if (vexRT[Ch3] > 0)
					motor[motorRight] = gear * 100 * (1 / (1 + pow(2.718, -((vexRT[Ch3]/8) - 6))));
				else if (vexRT[Ch3] < 0)
					motor[motorRight] = -gear * 100 * (1 / (1 + pow(2.718, -((-vexRT[Ch3]/8) - 6))));
				else
					motor[motorRight] = 0;

				if(vexRT[Ch2]<20 && vexRT[Ch2]>-20 && vexRT[Btn6D]==1){
		    	motor[motorLeft] = -20;
		    }
				else if (vexRT[Ch2] > 0)
					motor[motorLeft] = -gear * 100 * (1 / (1 + pow(2.718, -((vexRT[Ch2]/8) - 6))));
				else if (vexRT[Ch2] < 0)
					motor[motorLeft] = gear * 100 * (1 / (1 + pow(2.718, -((-vexRT[Ch2]/8) - 6))));
				else
					motor[motorLeft] = 0;

			}
		}

		//toggle autonomous

		bool autoActive = false;

		if (vexRT[Btn7L] != prev7L)
		{
			if(vexRT[Btn7L] == 1) {
				autoActive = true;
				clearTimer(timer3);
			}
			prev7L = vexRT[Btn7L];
		}

		//autonomous - 2 sensors

		int blackDetected = 1;
		int whiteDetected = 0;
		int limitPressed = 0;
		int forwardSpeed = 50;
		int turnSpeed = 40;
		if(time1[timer2] >= 5000)
				motor[autoRelease] = 128;

		while (autoActive)
		{

			if ((time1[timer3] >= 7000 && SensorValue(limitSwitch) == limitPressed) || time1[timer3] >= 17000) {
				// stop drive when limit switch detected
				motor[motorLeft] = 0;
				motor[motorRight] = 0;

				// drop piece and end auto
				motor[autoRelease] = 0;
				autoActive = false;

				// start timers
				clearTimer(timer2);
				//clearTimer(timer3); - for 5 second delay after pressed

				// if the timer has been going for longer than 5 sec, release the piece
				/*while (SensorValue(limitSwitch) == limitPressed)
				{
					if (time1(timer3) >= 5000) {
						motor[autoRelease] = -128;
						autoActive = false;
					}
				}*/
			}
			else
			{
				// both detect black, go forward
				if (getIrReading(irLeft) == blackDetected && getIrReading(irRight) == blackDetected)
				{
					motor[motorLeft] = forwardSpeed;
					motor[motorRight] = -forwardSpeed;
				}
				// white detected on left, turn left
				else if (getIrReading(irLeft) == whiteDetected && getIrReading(irRight) == blackDetected)
				{
					motor[motorLeft] = 1 * turnSpeed;
					motor[motorRight] = 0.7 * turnSpeed;
				}
				// white detected on right, turn right
				else if (getIrReading(irRight) == whiteDetected && getIrReading(irLeft) == blackDetected)
				{
					motor[motorLeft] = -0.7 * turnSpeed;
					motor[motorRight] = -1 * turnSpeed;
				}
				// both detect white, go backwards
				else
				{
					motor[motorLeft] = -forwardSpeed*0.75;
					motor[motorRight] = forwardSpeed*0.75;
				}
			}

			// turn off auto with 7L
			if (vexRT[Btn7L] != prev7L)
			{
				if(vexRT[Btn7L] == 1){
					autoActive = false;
					motor[autoRelease] = 128;
				}
				prev7L = vexRT[Btn7L];
			}
		}
	}
}
