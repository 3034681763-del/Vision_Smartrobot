#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "zf_common_headfile.h"

typedef struct
{
    float SumError;  //误差累积

    float Kp;      //比例系数
    float Ki;      //积分系数
    float Kd;      //微分系数
      float K_gory;


    float LastError;  //上一次误差
    float LLastError; //上上次误差
}PID;


void IncPIDInit(PID *sptr);

void my_motor_init();

void motor_set_duty(int16 left_PWM,int16 right_PWM,int16 behind_PWM);

int16 IncPIDCalc(PID *sptr,int16 Setpoint,int16 Turepoint);
int16 LocP_DCalc(PID*sptr,int16 Setpoint,int16 Turepoint);
extern int16 Target_Speed_cs;
void speed();
void speed_1();
#endif 
