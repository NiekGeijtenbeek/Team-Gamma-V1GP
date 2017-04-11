#pragma config(Sensor, S1,     light_L,        sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Sensor, S4,     light_R,        sensorLightActive)
#pragma config(Motor,  motorB,          motor_L,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          motor_R,       tmotorNXT, PIDControl, encoder)
#pragma platform(NXT)

const int kMaxSizeOfMessage = 30;
const int INBOX = 5;

char direction;


task drive() {
	float Kp = 2;																			//proportional gain
	float Ki = 0.003;																//integral gain
	float Kd = 400;																				//derivative gain
	int white_Value = 60;
	int black_Value = 32;
	int offset = ((white_Value + black_Value)/2);			//average white en black values
	int Tp = 35;																			//power (-100 t/m 100)
	float integral = 0;
	float last_Error = 0;
	float derivative = 0;

	while(true)
	{

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
//		playTone(2000, 3);
	}
}

int check_crossroad() {
	if(((SensorValue(light_L) < 42) && ((SensorValue(light_R) > 55) && (SensorValue(light_R) < 63))) || ((SensorValue(light_L) < 42) && (SensorValue(light_R) < 40))){
		return 1;
	}
	else {
		return 0;
	}
}


void take_crossroad(char direction) {
		if (direction == 'l') {
		motor[motor_L] = 0;
		motor[motor_R] = 40;
		wait1Msec(500);
		waitUntil(SensorValue(light_L) < 40);
		}

		else if (direction == 'r') {
		motor[motor_L] = 40;
		motor[motor_R] = 0;
		wait1Msec(250);
		waitUntil(SensorValue(light_R) < 40);
		waitUntil(SensorValue(light_L) < 40);
		}
}

//void grid_move(int x, int y) {
//	for (int c = 0; c != x; c++) {
//	startTask(drive);
//	waitUntil(SensorValue(light_L) < 40 || SensorValue(light_R) < 40);
//	stopTask(drive);
//	take_crossroad('r');
//	}
//	startTask(drive);
//	waitUntil(SensorValue(light_L) < 40 || SensorValue(light_R) < 40);
//	take_crossroad('l');
//
//	startTask(drive);
//}

char bluetooth(){
	TFileIOResult nBTCmdRdErrorStatus;
	int nSizeOfMessage;
	ubyte nRcvBuffer[kMaxSizeOfMessage];

	nSizeOfMessage = cCmdMessageGetSize(INBOX);

	if (nSizeOfMessage > kMaxSizeOfMessage)
		nSizeOfMessage = kMaxSizeOfMessage;
	if (nSizeOfMessage > 0){
		nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
		nRcvBuffer[nSizeOfMessage] = '\0';
		string s = "";
		stringFromChars(s, (char *) nRcvBuffer);
		//displayCenteredBigTextLine(4, s);

		if(s == "LEFT"){ //0
			direction = 'l';
		}
		if(s == "RIGHT"){ //1
			direction = 'r';
		}
		if(s == "DOWN"){ //2

		}
		if(s == "UP"){ //3
			direction = 'u';
		}
		if(s == "FIRE"){ //4
			direction = 's';
		}
		if(s == "A"){ //5

		}
		if(s == "B"){ //6

		}
		if(s == "C"){ //7

		}

	}
	return direction;
}

task main(){
	while(true) {

		while(SensorValue(sonar) < 30){
			stopTask(drive);
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
		startTask(drive);
		int crosscode = check_crossroad();
		char direction = bluetooth();
		if (crosscode == 1) {
			stopTask(drive);
			wait1Msec(100);
			take_crossroad(direction);
			crosscode = 0;
			direction = 'x';
		}
	}
}
