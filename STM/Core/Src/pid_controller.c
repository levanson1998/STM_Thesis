/*
 * pid_controller.c
 *
 *  Created on: Jul 23, 2020
 *      Author: Son
 */

#include <pid_controller.h>
#include "math.h"


//volatile float PID_in[2];
float PID_out[2];
//float PID_P[2], PID_I[2], PID_D[2];
float PID_Kp[2], PID_Ki[2], PID_Kd[2];
float PID_Test[10];
float PID_pre_err[2], PID_ppre_err[2];
float PID_out_max=299.0f, PID_out_min=-299.0f, PID_T;
float PID1[2];

float error, PID_P, PID_I, PID_D;

void PID_Init(float *Kp, float *Ki, float *Kd, float Ts) {
	for (int i = 0; i < 2; i++) {
		PID_Kp[i] = *(Kp+i);
		PID_Ki[i] = *(Ki+i);
		PID_Kd[i] = *(Kd+i);
	}
	PID_T = Ts/1000.0f;
}



float * PID_Calculate(float *PID_in, float *PID_current){
//	float error, PID_P, PID_I, PID_D;
//	volatile float PID_out[2];

	for (int i=0;i<2; i++){
		PID_Test[i]=*(PID_in+i);
		error = (*(PID_current+i)-*(PID_in+i));
		PID_P = (float)(PID_Kp[i]*(error-PID_pre_err[i]));
		PID_I = (float)(0.5F*PID_Ki[i]*PID_T*(error+PID_pre_err[i]));
		PID_D = (float)(PID_Kd[i]*(error-2.0f*PID_pre_err[i]+PID_ppre_err[i])/PID_T);

		PID_out[i] = (PID_P + PID_I + PID_D + PID1[i]);

		if (PID_out[i]>PID_out_max){
			PID_out[i]=PID_out_max;
		}
		else if (PID_out[i]<PID_out_min){
			PID_out[i]=PID_out_min;
		}
		PID1[i] = PID_out[i];
		PID_ppre_err[i]=PID_pre_err[i];
		PID_pre_err[i]=error;
	}
	return PID_out;
}

