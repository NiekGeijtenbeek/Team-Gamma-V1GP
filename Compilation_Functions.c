#pragma config(Sensor, S1,     light_L,        sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Sensor, S4,     light_R,        sensorLightActive)
#pragma config(Motor,  motorB,          motor_L,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          motor_R,       tmotorNXT, PIDControl, encoder)

int interupt = 0;

task drive() {
	float Kp = 2;																			//proportional gain
	float Ki = 0.003;																//integral gain
	float Kd = 400;																				//derivative gain
	int white_Value = 60;
	int black_Value = 32;
	int offset = ((white_Value + black_Value)/2);			//average white en black values
	int Tp = 50;																			//power (-100 t/m 100)
	float integral = 0;
	float last_Error = 0;
	float derivative = 0;

	while(true){
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
		playTone(2000, 3);
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

int take_crossroad(char direction) {
		if (direction == 'l') {
		motor[motor_L] = 0;
		motor[motor_R] = 80;

		wait1Msec(300);
		waitUntil(SensorValue(light_L) < 38);
		}

		else if (direction == 'r'){
		motor[motor_L] = 80;
		motor[motor_R] = 0;

		wait1Msec(300);
		waitUntil(SensorValue(light_L) < 38);
		}

		return 0;
}

task main(){
	while(true || interupt == 0) {
		startTask(drive);
		int crosscode = check_crossroad();
		if (crosscode == 1) {
			stopTask(drive);
			wait1Msec(100);
			take_crossroad('r');
			crosscode = 0;
		}
	}
}
