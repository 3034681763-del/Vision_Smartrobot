#ifndef __my_PID_h__
#define __my_PID_h__

#include "zf_common_headfile.h"




//void IncPIDInit(PID *sptr);
float Angle_PID(float Target_Angle, float Angle);
float smooth_speed_realtime (float current_speed, float *history_speed, int size);
void w_control();
float smooth_speed_realtime (float current_speed, float *history_speed, int size);
float PID_Angle_(float Target_Angle, float Angle);
//int16 LocP_DCalc(PID*sptr,int16 Setpoint,int16 Turepoint);
float Angle_PID_1(float Target_Angle_1, float Angle_1);
float Angle_PID_2(float Target_Angle_2, float Angle_2);
float Angle_PID_3(float Target_Angle_3, float Angle_3);
float Angle_PID_w(float Target_Angle_w, float Angle_w);
extern int16  w;
float Turn_w(float Target_Angle_w, float Angle_w);
#endif