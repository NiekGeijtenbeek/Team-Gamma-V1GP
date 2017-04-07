#pragma config(Sensor, S1,     light_L,        sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Sensor, S4,     light_R,        sensorLightActive)
#pragma config(Motor,  motorB,          motor_L,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          motor_R,       tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(NXT)

//long nLastXmitTimeStamp = nPgmTime;
//long nDeltaTime         = 0;

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;

/*
int LightValues[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int BounceTimerCounter = 0;

void shiftValue(int a);
int LargestNumber();
*/

task main()
{

  TFileIOResult nBTCmdRdErrorStatus;
  int nSizeOfMessage;
  ubyte nRcvBuffer[kMaxSizeOfMessage];

	float Kp = 3;																			//proportional gain
	float Ki = 0.004;																//integral gain
	float Kd = 520;																				//derivative gain
	int white_Value = 60;
	int black_Value = 32;
	int offset = ((white_Value + black_Value)/2);			//average white en black values
	int Tp = 30;																			//power (-100 t/m 100)
	float integral = 0;
	float last_Error = 0;
	float derivative = 0;

	while(true)
	{
		playTone(440,5);			//play sound while driving

		float light_Value = SensorValue(light_L);													//light reading
		float error = light_Value - offset;																//calculate error
		integral += error;
		derivative = error - last_Error;																	//calculate derivative
		float turn = Kp*error + Ki*integral + Kd*derivative;							//change the motors power
		float powerR = Tp + turn;																					//powerlevel for motor R
		float powerL = Tp - turn;																					//powerlevel for motor L
		motor[motor_R] = powerR;																					//motor R command
		motor[motor_L] = powerL;																					//motor L command
		last_Error = error;

		if(((SensorValue(light_L) < 42) && ((SensorValue(light_R) > 55) && (SensorValue(light_R) < 63))) || ((SensorValue(light_L) < 42) && (SensorValue(light_R) < 40))){
			playTone(2000, 8);

		//	motor[motor_R] = powerR;																					//motor R command
		//	motor[motor_L] = powerL;

		//	wait1Msec(250);

			motor[motor_L] = 0;
			motor[motor_R] = 80;

			wait1Msec(300);
			waitUntil(SensorValue(light_L) < 38);
		}

		while(SensorValue(sonar) < 25){

			if(motor[motor_L] < 2 || motor[motor_R] < 2){
				motor[motor_L] = 0;
				motor[motor_R] = 0;
			}else{
				for(int i = 30; i > 0; i--){
				 	motor[motor_L] = i;
					motor[motor_R] = i;
					wait1Msec(10);
				}
			}
		}


//		eraseDisplay();
//		for(int c = 0; c<8; c++)
//		{
//			nxtDisplayString(c, "%d", LightValues[c]);
//		}
//		shiftValue(SensorValue[light_L]);
//
	//	if(LightValues[LargestNumber()] > LightValues[0] + 10 && BounceTimerCounter == 0)
	//	{
	//		playTone(2000, 10);
	//		BounceTimerCounter = 25;
	//	}
	//	if(BounceTimerCounter > 0){ //Voorkomt flow naar negatief naar max
	//		BounceTimerCounter--;
	//	}
		//wait1Msec(1);

		    // Check to see if a message is available

    nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage)
      nSizeOfMessage = kMaxSizeOfMessage;
    if (nSizeOfMessage > 0){
    	nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    	nRcvBuffer[nSizeOfMessage] = '\0';
    	string s = "";
    	stringFromChars(s, (char *) nRcvBuffer);
    	displayCenteredBigTextLine(4, s);
    }
   // wait1Msec(100);

  }
  return;
}
/*
void shiftValue(int a)
{
	int TempValues[32] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	for(int c = 0; c<32; c++)
	{
		TempValues[c] = LightValues[c];
	}
	LightValues[0] = a;

	for(int c = 1; c<32; c++)
	{
		LightValues[c] = TempValues[c-1];
	}
}

int LargestNumber()
{
	int temp = 0;

	for (int c= 0; c<32; c++)
	{
		if(LightValues[temp] < LightValues[c])
		{
			temp = c;
		}
	}
	return temp;
}
*/