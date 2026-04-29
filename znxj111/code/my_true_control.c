#include "zf_common_headfile.h"
#include <math.h>

//#include "isr.h"
uint8 Control_Mode = 0;    
uint8 Correct_Mode = 0;
uint8 gpio_status;
float T_M_re=0;
float T_M_case_1=0;
float	my_rate=0;
float angle_now=0;
uint16 cnt = 0;
uint16 ant = 0;
uint16 mnt = 0;
uint32 mnt_new = 0;
uint16 mnt_new_detail = 0;
uint32 mnt_new_detail_2 = 0;
float T_M_case_push=0;
uint16 number=0;
uint8 Object=0;
int object[10]={0};
float T_M_case_test=0;
int num;
int num2;
int num3;
int num4;
// ¿ØÖÆº¯Êý
void my_motor_control()
{

	
    switch(Control_Mode)
    {
			case 0:
			motor_control_trail();
				if(my_rate>=10.0)			//10
					{
					T_M_case_1=T_M;
					Control_Mode=12;
				 }
			 break;
			case 1:
				
				motor_control_position();
				Control_Mode=2;
			 break;
				case 2:	
					motor_control_position();
						if(abs(filtered_box_x -70)<8&&abs(filtered_box_y -45 )<7  ) //80 55 5// 70 42 8// 70 55 5//70 46 7//75 55 7//75 46 7
					{
					is_rotating=true;
					Object_load_new();
					angle_now=T_M;
//						 Control_Mode=3;  
						Control_Mode=7;
//						motor_set_duty(0,0,0);
//						Control_Mode=7;
					}
				 break;	
			case 3:
//				Jugde_Direction();
			Revolove_Box(1);
					if(abs(T_M- Angle_w_recond)<=190&&abs(T_M- Angle_w_recond)>=170)
				{
					is_rotating=false;
					T_M_case_push=T_M	;
					Control_Mode=9;
					
				}	
				break;
       case 4:         
				is_rotating=false;
//				 speed_change=-20;
//				 speed_control();
			 motor_set_duty(-1900,1900,0);
////			 backward();
//			 forward_new();
//	Speed_pwm_left    += IncPIDCalc(&L_SpeedPID, Target_Speed_A, encoder_data[1]);////50
//	Speed_pwm_right   += IncPIDCalc(&R_SpeedPID, Target_Speed_B, encoder_data[0]);
//	Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID, Target_Speed_C, encoder_data[2]);

//	motor_set_duty(Speed_pwm_left, Speed_pwm_right, Speed_pwm_behind);
//			 forward_new();
			 gpio_status = gpio_get_level(C26);
			
				 if( gpio_status==0 ) 
				 {
					  ant=0;
						Control_Mode=5;
				 }
            break;
        case 5:                         
//					speed_control_Debug();
				motor_set_duty(-1300,1300,0);
//				forward();
////				backward_new();
//				backward_new();
			   gpio_status = gpio_get_level(C26);
			 if( gpio_status==1)
			 {
				  T_M_re=T_M;
				  Control_Mode=6;
			 }
			
            break;
         case 6:   
				 	Position_control();
				 if(abs(T_M-T_M_case_1)<=10||abs(abs(T_M-T_M_case_1)-360)<=10)
				 {
					 Control_Mode=0;
				 }
         break;    
				 case 7:
       motor_set_duty(0,0,0);
				 num++;
				 if(num>1000){
				 Control_Mode=3; 
         			num=0;	 }
				 break;
				 case 8:
					 motor_set_duty(0,0,0);
				 Control_Mode=3;
				 break;
				 case 9:
					 motor_set_duty(0,0,0);

				 num2++;
				 if(num2>400){
					  Control_Mode=10;
					 num2=0;
				 }
				 break;
				 case 10:
//					 motor_control_position();
				 motor_control_position_2();
						if(abs(filtered_box_x -75)<7&&abs(filtered_box_y -55 )<7  ) //80 55 5// 70 42 8// 70 55 5
					{
					Control_Mode=11;
					}
					break;
				 case 11:
					 motor_set_duty(0,0,0);
				   num3++;
					 if(num3>200){
					  Control_Mode=4;
					 num3=0; 
				 }
					 break;
				 case 12:
					 motor_set_duty(0,0,0);
				 num4++;
				 if(num4>200){
				Control_Mode=1;
				 num4=0;}
				 break;
					 
					 
  	}
}