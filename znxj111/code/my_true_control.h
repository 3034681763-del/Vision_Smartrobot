#ifndef __my_true_control_H__
#define __my_true_control_H__
#include "zf_common_headfile.h"

void my_motor_control();
extern uint8 gpio_status;
void rotate_Direction();
extern float my_rate;
extern float T_M_re;
extern float T_M_case_1;
extern float angle_now;

extern uint16 cnt ;
extern uint16 ant;
extern float T_M_case_push;
extern uint8 Object;
extern int object[10];
extern uint8 Control_Mode;
extern uint8 Correct_Mode;
extern float T_M_case_test;
extern uint16 number;
extern uint16 mnt ;
extern uint32 mnt_new;
extern uint16 mnt_new_detail;
#endif