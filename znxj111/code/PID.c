#include "zf_common_headfile.h"
#include <math.h>


// 车和箱子的位置

float Kp_turn_A=0.4,Kd_turn_A=0,Ki_turn_A=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8       Kp_A=0.8,Kd_A=0,Ki_A=0  
float Angle_turn_PID_x(float Target_Angle, float Angle)
{	
	
	float err = Target_Angle - Angle;
	static float last_err, integral;
	float out = Kp_turn_A*err +Kd_turn_A*(err - last_err) + Ki_turn_A*integral;
	last_err = err;
	integral += err;
	integral = func_limit(integral,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out;
}

// 纠正车和箱子的位置

float Kp_turn_B=0.9,Kd_turn_B=0,Ki_turn_B=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8       Kp_A=1.8,Kd_A=0,Ki_A=0
float Angle_turn_PID_y(float Target_Angle, float Angle)
{	
	float err = Target_Angle - Angle;
	static float last_err, integral;
	float out = Kp_turn_B*err +Kd_turn_B*(err - last_err) + Ki_turn_B*integral;
	last_err = err;
	integral += err;
	integral = func_limit(integral,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}

// 控制车子旋转

float Kp_A_turn_w=0.25,Kd_A_turn_w=0,Ki_A_turn_w=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8  Kp_A_3=0.2,Kd_A_3=10,Ki_A_3=0
float Angle_PID_turn_w(float Target_Angle_3, float Angle_3)
{	
	//Angle = slidingFilter(Angle);
	float err_3 = Target_Angle_3 - Angle_3;
	static float last_err_3, integral_3;
	float out_3 = Kp_A_turn_w*err_3 +Kd_A_turn_w*(err_3 - last_err_3) + Ki_A_turn_w*integral_3;
	last_err_3 = err_3;
	integral_3 += err_3;
	integral_3 = func_limit(integral_3,1000); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
	out_3 = func_limit(out_3,1000);   //70
    return out_3;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}