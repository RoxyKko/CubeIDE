#include "pid.h"
float  Kp,Ki,Kd;		

/**************************************************************************
函数功能：绝对值计算
入口参数：
返回  值：
**************************************************************************/
float abs_f(float x)
{
	if(x>=0)
		return x;
	else
		return -x;
}

/**************************************************************************
函数功能：PID计算
入口参数：目前温度，目标温度
返回  值：PWM值
**************************************************************************/
int PID_Calculation(short  value,double  Target)
{
	Kp = 100;
	Ki = 1;
	Kd = 0;
	float error_x,Integral_error_x,Last_error_x;
	int PWM;
	error_x =(int)abs_f(value-Target);
	Integral_error_x += error_x;
	PWM = (int)(Kp * error_x + Ki * Integral_error_x + Kd * (error_x - Last_error_x));
	Last_error_x = error_x;
	return PWM;
}
/**************************************************************************
函数功能：PWM限幅
入口参数：PWM值
返回  值：PWM值
**************************************************************************/
int  Limiting_amplitude(int PWM)
{	
	if(PWM<0) return 0;	
	if(PWM>1800) return 1800;	
}
