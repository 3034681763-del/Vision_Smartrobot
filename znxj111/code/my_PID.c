#include "zf_common_headfile.h"
#include <math.h>

int16  w = 0;  //x、 y轴分速度,车绕几何中心的角速度


// 纠正车和箱子的位置

float Kp_A=-0.34,Kd_A=0,Ki_A=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8       Kp_A=0.8,Kd_A=6.4,Ki_A=0// ziheng update 0.6 0 0 //-0.3
float Angle_PID(float Target_Angle, float Angle)
{	
	//Angle = slidingFilter(Angle);
	float err = Target_Angle - Angle;
	static float last_err, integral;
	float out = Kp_A*err +Kd_A*(err - last_err) + Ki_A*integral;
	last_err = err;
	integral += err;
	integral = func_limit(integral,200); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}

float Kp_A_1=0.27 ,Kd_A_1=0,Ki_A_1=0; // 20/95/0  Kp_A = 2.9,Kd_A = 10 ziheng update 1.8 0 0 //0.25
float Angle_PID_1(float Target_Angle_1, float Angle_1)
{	
	//Angle = slidingFilter(Angle);
	float err_1 = Target_Angle_1 - Angle_1;
	static float last_err_1, integral_1;
	float out_y = Kp_A_1*err_1 +Kd_A_1*(err_1 - last_err_1) + Ki_A_1*integral_1;
	last_err_1 = err_1;
	integral_1 += err_1;
	integral_1 = func_limit(integral_1,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out_y;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}

//角度环（中环pid） 
float Kp_angle=2.3,Kd_angle=0,Ki_angle=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8    0.6
float PID_Angle_(float Target_Angle, float Angle)
{	
	//Angle = slidingFilter(Angle);
	float err = Target_Angle - Angle;
	static float last_err, integral;
	float out = Kp_angle*err +Kd_angle*(err - last_err) + Ki_angle*integral;
	last_err = err;
	integral += err;
	integral = func_limit(integral,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}


//控制车的转向

float Kp_A_2=0.05,Kd_A_2=0.01,Ki_A_2=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8
float Angle_PID_2(float Target_Angle_2, float Angle_2)
{	
	//Angle = slidingFilter(Angle);
	float err_2 = Target_Angle_2 - Angle_2;
	static float last_err_2, integral_2;
	float out_y = Kp_A_2*err_2 +Kd_A_2*(err_2 - last_err_2) + Ki_A_2*integral_2;
	last_err_2 = err_2;
	integral_2 += err_2;
	integral_2 = func_limit(integral_2,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out_y;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}

//角度闭环控制车行走

float Kp_A_3=0.35,Kd_A_3=8,Ki_A_3=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8  Kp_A_3=0.1,Kd_A_3=0.0,Ki_A_3=0.01
float Angle_PID_3(float Target_Angle_3, float Angle_3)
{	
	//Angle = slidingFilter(Angle);
	float err_3 = Target_Angle_3 - Angle_3;
	static float last_err_3, integral_3;
	float out_3 = Kp_A_3*err_3 +Kd_A_3*(err_3 - last_err_3) + Ki_A_3*integral_3;
	last_err_3 = err_3;
	integral_3 += err_3;
	integral_3 = func_limit(integral_3,1000); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
	out_3 = func_limit(out_3,1000);   //70
    return out_3;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}



float Kp_w=8.0,Kd_w=15,Ki_w=0; // 20/95/0  Kp_A = 8,Kd_A = 15
float Angle_PID_w(float Target_Angle_w, float Angle_w)
{	
	//Angle = slidingFilter(Angle);
	float err_w = Target_Angle_w - Angle_w;
	static float last_err_w, integral_w;
	float out_w = Kp_w*err_w +Kd_w*(err_w - last_err_w) + Ki_w*integral_w;
	last_err_w = err_w;
	integral_w += err_w;
	integral_w = func_limit(integral_w,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out_w;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}
//定位w

float Kp_turn=1.2,Kd_turn=8,Ki_turn=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8
float Turn_w(float Target_Angle_w, float Angle_w)
{	
	//Angle = slidingFilter(Angle);
	float err_w = Target_Angle_w - Angle_w;
	static float last_err_w, integral_w;
	float out_w = Kp_turn*err_w +Kd_turn*(err_w - last_err_w) + Ki_turn*integral_w;
	last_err_w = err_w;
	integral_w += err_w;
	integral_w = func_limit(integral_w,100); //积分限幅
	//return (out>=0) ? (8*sqrt(out)) : (-8*sqrt(func_abs(out)));
//	out = func_limit(out,100);   //70
    return out_w;
 //  w = Angle_PID(angle_now + angle_turn, Gyro_Angle.Zdata);
}
void w_control()
{
	 
	 w = Angle_PID(0, T_M);
	
	
	
}
float smooth_speed_realtime (float current_speed, float *history_speed, int size)
{
	float sum =0;
float average_speed;
for (int i =0; i<size; i++)
	{
   sum += history_speed[i];
  }
  average_speed = sum / size;
for (int i = 0; i< size - 1; i++) 
	{
  history_speed[i]= history_speed[i + 1];
  }
	history_speed[size - 1] = current_speed; 
	sum = 0;
 for (int i = 0; i< size; i++)
	{
  sum += history_speed[i];
  }
  return sum / size;
}





