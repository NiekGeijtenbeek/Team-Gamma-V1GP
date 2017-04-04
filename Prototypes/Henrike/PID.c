#pragma config(Sensor, S1, 		light_L, 		sensorLightActive)
#pragma config(Sensor, S4, 		light_R, 		sensorLightActive)
#pragma config(Motor, motorB, 		motor_L,		tmotorNXT, PIDControl, encoder)
#pragma config(Motor, motorC, 		motor_R,		tmotorNXT, PIDControl, encoder)

task main(){

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
