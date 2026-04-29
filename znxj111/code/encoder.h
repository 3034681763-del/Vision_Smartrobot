#ifndef _encoder_h_
#define _encoder_h_

#include "zf_common_headfile.h"

void my_encoder_init(void);
void pit_handler_0 (void);
void Turn(float Target_slope, float actual_slope);
void forward();
void backward();
void forward_new();
void backward_new();
extern int16 encoder_data[4];
extern int16 encoder[4];
extern float aim_speed;
extern float aim2_speed;
//void Kinematic_Analysis(float Vx,float Vy,float V_angle);
extern float Target_Speed_C;
extern float Target_Speed_A;
extern float Target_Speed_B;
extern float Vx;
extern float Vy;
extern float V_angle;
extern float target_slope;
extern float speed_behind;
extern float speed_left;
extern float speed_right;
extern float distance;

#endif