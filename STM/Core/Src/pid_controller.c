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
float PID_out_max=299.0f, PID_out_min=0.0f, PID_T;
float PID1[2];

float error, PID_P[2], PID_I[2], PID_D[2];

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
	PID_Test[2] = *(PID_current);

	for (int i=0;i<2; i++){
		PID_Test[i]=*(PID_in+i);
		error = *(PID_in+i)-*(PID_current+i)/**enc[i*2+1]*/;
		PID_P[i] = (float)(PID_Kp[i]*(error-PID_pre_err[i]));
		PID_I[i] = (float)(0.5F*PID_Ki[i]*0.005f*(error+PID_pre_err[i]));
		PID_D[i] = (float)(PID_Kd[i]*200.0f*(error-2.0f*PID_pre_err[i]+PID_ppre_err[i]));

		PID_out[i] = (PID_P[i] + PID_I[i] + PID_D[i]) + PID_out[i];

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

/*
	E0 = (10.0F - *PID_current);
	A0 = PID_Kp[0] + PID_Ki[0]/400.0F + PID_Kd[0]*200.0F;
	A1 = -PID_Kp[0] + PID_Ki[0]/400.0F - 400.0F*PID_Kd[0];
	A2 = PID_Kd[0]*200.0F;
	Aout = A0*E0 + A1*E1 + A2*E2 + Aout1;

	E2 = E1;
	E1 = E0;

	if (Aout>PID_out_max){
		Aout=PID_out_max;
	}
	else if (Aout<PID_out_min){
		Aout=PID_out_min;
	}
	Aout1 = Aout;
	PID_out[0] = Aout;
	PID_out[1] = 0;
*/

//	PID_Test[5] += PID_out[0];
	return PID_out;
}
