#pragma config(Sensor, S1, 		light_L, 		sensorLightActive)
#pragma config(Sensor, S4, 		light_R, 		sensorLightActive)
#pragma config(Motor, motorB, 		motor_L,		tmotorNXT, PIDControl, encoder)
#pragma config(Motor, motorC, 		motor_R,		tmotorNXT, PIDControl, encoder)

task main(){

	float Kp = 3.6;																			//proportional gain
	float Ki = 0.003168;																//integral gain
	float Kd = 0;																				//derivative gain
	int white_Value = 50;
	int black_Value = 40;
	int offset = ((white_Value + black_Value)/2);			//average white en black values
	int Tp = 30;																			//power (-100 t/m 100)
	float integral = 0;
	float last_Error = 0;
	float derivative = 0;

	while (true){
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
	}

}
