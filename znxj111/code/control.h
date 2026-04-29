#ifndef _control_h_
#define _control_h_

#include "zf_common_headfile.h"


extern bool is_rotating;

void PID_int(void);
void motor_control_position();
void Turn(float Target_slope, float actual_slope);
void forward();
void backward();
int16 range_protect(int16 duty, int16 min, int16 max);
void Kinematic_Analysis(float Vx,float Vy,float V_angle);
void Turn(float Target_slope, float actual_slope);
extern int16 target_speed;
void Position_control();
void Yaw_astrict(float *data);
void turn_control();
void speed_control();
void motor_control_trail();
void Object_load();
float Turn_w_new(float Target_Angle_w, float Angle_w);
//extern PID TurnPID;
//extern PID SpeedPID;
//extern PID L_SpeedPID;
//extern PID R_SpeedPID;
//extern PID B_SpeedPID;
extern int16 Speed_pwm_left  ;     //左轮速度环PWM（C车用）
extern int16 Speed_pwm_right   ;     //左轮速度环PWM（C车用）
extern int16 Speed_pwm_behind  ;     //后轮速度环PWM（C车用）
extern int16 target_speed_1;
extern float angle_now_count;
extern float angle_now_angle;
extern int16 turn_angle;
void speed_control_Debug();
extern int16 speed_change;
extern float Target_Angle_w_count;
float Turn_w_new_2(float Target_Angle_w, float Angle_w);
void motor_control_position_2();
extern float angle_test;
float Turn_w_TLY(float Target_Angle_w, float Angle_w);
void speed_control_Debug_2();
void Kinematic_Analysis_new(float Vx,float Vy,float V_angle);
void Kinematic_Analysis_new(float Vx,float Vy,float V_angle);
void qianjin();
extern float Angle_w_recond;
extern float Angle_w_recond_2;
extern uint8 dir;
float Turn_w_TLY_2(float Target_Angle, float Angle);
#endif