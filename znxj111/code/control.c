#include "zf_common_headfile.h"
#include <math.h>

PID TurnPID;
PID SpeedPID;
PID L_SpeedPID;
PID R_SpeedPID;
PID B_SpeedPID;
float target_angle = 0; 
int16 Speed_pwm_left  =0 ;    
int16 Speed_pwm_right =0  ;     
int16 Speed_pwm_behind =0  ;    
float Vx = 0, Vy = -64,V_angle = 0;  
int16 target_speed;
int16 Speed_left  =0 ;    
int16 Speed_right =0  ;    
int16 Speed_behind =0  ;    
int16 Position_left  =0 ;    
int16 Position_right =0  ;     
int16 Position_behind =0  ;    
int16 x_error  =0 ;    
int16 y_error =0  ;     
int16 car_x_error =0 ;     
int16 car_y_error =0  ;    
int16 car_w_error =0  ;    
int16 target_speed_1=40;
uint8 gpio_status_new;
int16 turn_angle;
float angle_now_angle=0;

float angle_now_count=0;
float Target_Angle_w_count=0;

int16 speed_change;

float angle_test=0;
float Angle_w_recond=0;
float Angle_w_recond_2=0;

uint8 rotate_direction = 0;	
bool is_rotating = false;	


void PID_int(void)
{
    SpeedPID.Kp=120;     //位置环  0.7
    SpeedPID.Ki=0 ;     //
    SpeedPID.Kd=80;

    L_SpeedPID.Kp=100; //4  //左轮速度环PID参数（速度环20ms）//100
    L_SpeedPID.Ki=0.75;//1  0.75                  zheng
    L_SpeedPID.Kd=0;                                                     

    R_SpeedPID.Kp=100;   //右速度环PID参数（速度环20ms）//100 7.20 zijigenggai
    R_SpeedPID.Ki=0.75;  //      0.75                       fu 10 0.1
    R_SpeedPID.Kd=0;  

    TurnPID.Kp=0;       //转向环PID参数 
    TurnPID.Ki=0;                                                     
    TurnPID.Kd=0;                  
	
	 B_SpeedPID.Kp=100;   //后速度环PID参数（速度环20ms）
    B_SpeedPID.Ki=0.75;  //                                    
    B_SpeedPID.Kd=0;  
}
//循迹
void   motor_control_trail()
{
	  pit_handler_0 ();
	
	if(Island_State != 0 
		|| Control_Mode != 0)
	{
		Vy = -20;
	}
	else if(Island_State == 3 || Island_State == 4||Island_State==2)
	{
		Vy = -20;
	}
	else if(my_rate>=1.6)
	{
			Vy = -22 ;
 	}
	else if(my_rate>=3.3)
	{	
		Vy = -25;
	}
	else 
	{
		 Vy = -35;
	}
// 进环
	if(Island_State == 2
		&& Right_Island_Flag_new == 1)
	{
		Turn(0.6,Err_Sum()*0.22); //0.22 //1.1
	}
	if(Island_State == 2
		&& Left_Island_Flag_new == 1)
	{
		Turn(-1.1,Err_Sum()*0.22);//1
	}
	//环内
	else if(Island_State == 3
		&& Right_Island_Flag_new == 1)
	{
		Turn(3,Err_Sum()*0.27);//5.2
	}
	else if(Island_State == 3
		&& Left_Island_Flag_new == 1)
	{
		Turn(-3,Err_Sum()*0.27);//5.2
		//Tracking_1_Turn_PD(0,Err_Sum_INisland_L()*0.27, car_speed);
	}
	//出环
	else if(Island_State == 4
		&& Right_Island_Flag_new == 1)
	{
		Turn(3.5,Err_Sum()*0.24); //0.24//3
	}
	else if(Island_State == 4
		&& Left_Island_Flag_new == 1)
	{
		Turn(-3,Err_Sum()*0.24);//2.4
	}
	else
	{
		Turn(0,Err_Sum()*0.27);	//循迹//0.27
	}
	Speed_pwm_left  += IncPIDCalc(&L_SpeedPID,Target_Speed_A ,encoder_data[1]);
	Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);  
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind );
    
}

void Revolove_Box(uint8 dir)
{
	is_rotating = true;
		switch(dir)
	{
		case 1: 
			angle_now_count=angle_now + 105;//130
			break;
		case 2: 
			angle_now_count=angle_now - 105;

			break;
	}
	pit_handler_0 ();
	update_vision_data(MCX_uart_rx_buff[0]+MCX_uart_rx_buff[2]/2, MCX_uart_rx_buff[1]+MCX_uart_rx_buff[3]/2);

			 // Yaw_astrict(&angle_now_count);
//		car_x_error =Angle_turn_PID_x(70, filtered_box_x);
//	  car_y_error =-Angle_turn_PID_y(42, filtered_box_y);//屏幕的y开始是60
 		w =  - Turn_w_new(angle_now_count, T_M);
//	 Kinematic_Analysis_new(car_x_error, car_y_error, w);
	
// 		w =  - Turn_w_new(angle_now_count, T_M);
	 Kinematic_Analysis_new(0, 0 , w);
	Speed_pwm_left    += IncPIDCalc(&L_SpeedPID, Target_Speed_A, encoder_data[1]);////50
	Speed_pwm_right   += IncPIDCalc(&R_SpeedPID, Target_Speed_B, encoder_data[0]);
	Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID, Target_Speed_C, encoder_data[2]);

	motor_set_duty(Speed_pwm_left, Speed_pwm_right, Speed_pwm_behind);
}

//定位
void motor_control_position()
{

	  pit_handler_0 ();
	
	   update_vision_data(MCX_uart_rx_buff[0]+MCX_uart_rx_buff[2]/2, MCX_uart_rx_buff[1]+MCX_uart_rx_buff[3]/2);

	car_x_error =Angle_PID(70 , filtered_box_x);
	car_y_error =-Angle_PID_1(45,filtered_box_y);//屏幕的y开始是60 FUDE
	car_w_error=-Turn_w_new_2(T_M_case_1, T_M); //FUDE
	
	Kinematic_Analysis(car_x_error ,car_y_error, car_w_error);
	Speed_pwm_left  += IncPIDCalc(&L_SpeedPID,Target_Speed_A ,encoder_data[1]);
	Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	
//	 Speed_pwm_left=range_protect(Speed_pwm_left, -4500,4000);//减速限幅（防止轮子反转太大导致假期望速度）-4500,9000
//    Speed_pwm_right=range_protect(Speed_pwm_right, -4500,4000);//减速限幅（防止轮子反转太大导致假期望速度）
//	Speed_pwm_behind=range_protect(Speed_pwm_behind, -4500,4000);
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
    
}
void motor_control_position_2()
{

	  pit_handler_0 ();
	
	   update_vision_data(MCX_uart_rx_buff[0]+MCX_uart_rx_buff[2]/2, MCX_uart_rx_buff[1]+MCX_uart_rx_buff[3]/2);

	car_x_error =Angle_PID(75, filtered_box_x);
	car_y_error =-Angle_PID_1(55,filtered_box_y);//屏幕的y开始是60 FUDE
	car_w_error=-Turn_w_new_2(T_M_case_1, T_M); //FUDE
	
	Kinematic_Analysis(car_x_error ,car_y_error, car_w_error);
	Speed_pwm_left  += IncPIDCalc(&L_SpeedPID,Target_Speed_A ,encoder_data[1]);
	Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	
//	 Speed_pwm_left=range_protect(Speed_pwm_left, -4500,4000);//减速限幅（防止轮子反转太大导致假期望速度）-4500,9000
//    Speed_pwm_right=range_protect(Speed_pwm_right, -4500,4000);//减速限幅（防止轮子反转太大导致假期望速度）
//	Speed_pwm_behind=range_protect(Speed_pwm_behind, -4500,4000);
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
    
}

void turn_control()
{
	  pit_handler_0 ();
	  update_vision_data(MCX_uart_rx_buff[0]+MCX_uart_rx_buff[2]/2, MCX_uart_rx_buff[1]+MCX_uart_rx_buff[3]/2);
		car_x_error =Angle_turn_PID_x (80,  filtered_box_x);
		car_y_error =-Angle_turn_PID_y(55, filtered_box_y);////屏幕的y开始是60
		angle_now_angle=angle_now+75;
		w= -Turn_w_new(angle_now_angle, T_M);
	Kinematic_Analysis_new(car_x_error ,car_y_error,w);
	
	Speed_pwm_left  += IncPIDCalc(&L_SpeedPID,Target_Speed_A ,encoder_data[1]);
	Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);

//	 Speed_pwm_left=range_protect(Speed_pwm_left, -4500,4000);//减速限幅（防止轮子反转太大导致假期望速度）-4500,9000
//    Speed_pwm_right=range_protect(Speed_pwm_right, -4500,4000);//减速限幅（防止轮子反转太大导致假期望速度）
//	Speed_pwm_behind=range_protect(Speed_pwm_behind, -4500,4000);
		
		motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);

}
void Position_control()
{
	  pit_handler_0 ();
		w=  Turn_w_TLY(T_M_case_1,T_M );//Target_Angle_w_count fude
		Kinematic_Analysis(0,0,w);
		 Speed_pwm_left  += IncPIDCalc(&L_SpeedPID,Target_Speed_A ,encoder_data[1]);//50
		 Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
		 Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID,Target_Speed_C ,encoder_data[2]);
    motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
}

//控制速度
void speed_control()
{
	  pit_handler_0 ();
	  car_x_error =Turn_w_TLY_2(80, MCX_uart_rx_buff[0]+MCX_uart_rx_buff[2]/2);
	  car_w_error=-Turn_w_new_2(T_M_case_push, T_M);
	  Kinematic_Analysis(car_x_error,speed_change,car_w_error);
	
  	Speed_pwm_left  += IncPIDCalc(&L_SpeedPID,Target_Speed_A ,encoder_data[1]);////50
	  Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	  Speed_pwm_behind  += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	  
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
}

void speed_control_Debug()
{

	  pit_handler_0 ();
	
  	Kinematic_Analysis(0,20,0);//y=60
	
	  Speed_pwm_left   += IncPIDCalc(&L_SpeedPID,Target_Speed_A,encoder_data[1]);//50
	  Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	  Speed_pwm_behind += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
    
}

void speed_control_Debug_2()
{

	  pit_handler_0 ();
	
	   update_vision_data(MCX_uart_rx_buff[0]+MCX_uart_rx_buff[2]/2, MCX_uart_rx_buff[1]+MCX_uart_rx_buff[3]/2);
	  
	

		car_x_error =Angle_turn_PID_x(80, filtered_box_x);
		car_y_error =-Angle_turn_PID_y(65, filtered_box_y);////屏幕的y开始是60
	   w= -Turn_w_TLY(400, T_M);
  	Kinematic_Analysis(0,0,w);
	  Speed_pwm_left   += IncPIDCalc(&L_SpeedPID,Target_Speed_A,encoder_data[1]);//50
	  Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	  Speed_pwm_behind += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
    
}
#define X_PARAMETER          (1/3.f)
#define Y_PARAMETER           (sqrt(3)/3.f)
#define L_PARAMETER            (2/3.f)


void Kinematic_Analysis(float Vx,float Vy,float V_angle)
{
    Target_Speed_C   = - L_PARAMETER*Vx + X_PARAMETER*V_angle;
    Target_Speed_A   = X_PARAMETER*Vx + Y_PARAMETER*Vy + X_PARAMETER*V_angle;
    Target_Speed_B   = X_PARAMETER*Vx - Y_PARAMETER*Vy + X_PARAMETER*V_angle;
}

//小车运动数学模型



void Kinematic_Analysis_new(float Vx,float Vy,float V_angle)
{
    Target_Speed_C   = - 2/3.f*Vx+5/3.f*V_angle;
    Target_Speed_A   = X_PARAMETER*Vx + Y_PARAMETER*Vy -1/3.f*V_angle;//A负数代表正转，B正数代表正转
    Target_Speed_B   = X_PARAMETER*Vx - Y_PARAMETER*Vy -1/3.f*V_angle;
}


//循迹转向函数

float Kp_T=-3, Kd_T=-10; //2.6， 1.5      -10   -5  Kp_T=-7, Kd_T=-10;keyixunji// 3.5 10
void Turn(float Target_slope, float actual_slope)
{
	static float err, err_last;
	err = Target_slope - actual_slope;
    V_angle = Kp_T*err + Kd_T*(err - err_last);
	err_last = err;
	PID_Angle_(0, V_angle);
	Kinematic_Analysis(Vx,Vy,V_angle);
}

void forward()
{
	Vx = 0;
    Vy = 20;
    V_angle = 0;
	Kinematic_Analysis(Vx,Vy,V_angle);
}
void backward_new()
{
//	Vx = 0;
//    Vy = 4;
//    V_angle = 0;
Kinematic_Analysis(0,-7,0);
	Speed_pwm_left   += IncPIDCalc(&L_SpeedPID,Target_Speed_A,encoder_data[1]);
	  Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	  Speed_pwm_behind += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
}
void forward_new()
{
//	Vx = 0;
//    Vy = -4;
//    V_angle = 0;
	Kinematic_Analysis(0,7,0);
	Speed_pwm_left   += IncPIDCalc(&L_SpeedPID,Target_Speed_A,encoder_data[1]);//50
	  Speed_pwm_right  += IncPIDCalc(&R_SpeedPID,Target_Speed_B,encoder_data[0]);
	  Speed_pwm_behind += IncPIDCalc(&B_SpeedPID,Target_Speed_C,encoder_data[2]);
	
	  motor_set_duty(Speed_pwm_left,Speed_pwm_right,Speed_pwm_behind);
}
void backward()
{
	Vx = 0;
    Vy = -20;
    V_angle = 0;
	Kinematic_Analysis(Vx,Vy,V_angle);
}
//void move(int16 angle, int8 speed)
//{
//    v_x = speed*cos(angle*3.14/180);
//    v_y = speed*sin(angle*3.14/180);
//    w = 0;
//}


void Yaw_astrict(float *data)
{
	
	if(*data>360)
	{
		*data=*data-360.0;
	}
	
	else if(*data<-360)
	{
		*data=*data+360.0;
	}
	
	
}
void Object_load()
{
	switch(Correct_Mode)
	{
			case 0:
				
			object[0]=Openart_uart_rx_buff[0];
			
			break;
			
			case 1:
			object[1]=Openart_uart_rx_buff[0];
			
			break;
			
			case 2:
			object[2]=Openart_uart_rx_buff[0];
			
			break;
			
			case 3:
			object[3]=Openart_uart_rx_buff[0];
			
			break;
			
			case 4:
			object[4]=Openart_uart_rx_buff[0];
			
			break;
			
			case 5:
			object[5]=Openart_uart_rx_buff[0];
			
			break;
			
			case 6:
			object[6]=Openart_uart_rx_buff[0];
			
			break;
			
			case 7:
			object[7]=Openart_uart_rx_buff[0];
			
			break;
		}
			
	}

float Turn_w_new(float Target_Angle_w, float Angle_w)
{	
    
    Target_Angle_w = fmod(Target_Angle_w, 360.0f);
	if(Target_Angle_w >= -360.0 && Target_Angle_w <= 360.0) 
    {
    Angle_w_recond=Target_Angle_w;
    }
    else if(Target_Angle_w>360.0)
    {
        Angle_w_recond=Target_Angle_w-360.0;
    }
    
    else if(Target_Angle_w<-360.0)
    {
        Angle_w_recond=Target_Angle_w+360.0;
    }
    if (Target_Angle_w < 0) 
			Target_Angle_w += 360.0f;
		Angle_w = fmodf(Angle_w, 360.0f);
    if (Angle_w < 0) Angle_w += 360.0f;
    
    float err_w = Target_Angle_w - Angle_w;
    
    if (err_w > 180.0f) {
        err_w -= 360.0f;
    } else if (err_w < -180.0f) {
        err_w += 360.0f;
    }
    static float last_err_w, integral_w;
    float out_w = 0.09 *err_w +0.0*(err_w - last_err_w) + 0 *integral_w; //0.3 0 0.005//0.01 0.005//0.13//0.1//0.09
    last_err_w = err_w;
    integral_w += err_w;
    integral_w = func_limit(integral_w, 2000); // 积分限幅
    
    return out_w;
}

float Turn_w_new_2(float Target_Angle_w, float Angle_w)
{	
    
    Target_Angle_w = fmod(Target_Angle_w, 360.0f);
    if (Target_Angle_w < 0) Target_Angle_w += 360.0f;
   
   
    float err_w = Target_Angle_w - Angle_w;
    
   
    if (err_w > 180.0f) {
        err_w -= 360.0f;
    } else if (err_w < -180.0f) {
        err_w += 360.0f;
    }

    static float last_err_w, integral_w;
    float out_w = 0.035 *err_w +0*(err_w - last_err_w) + 0*integral_w;//0.3//0.1 //0.03//0.05//0.03
    last_err_w = err_w;
    integral_w += err_w;
    integral_w = func_limit(integral_w, 100); // 积分限幅
    
    return out_w;
}

float Turn_w_TLY(float Target_Angle_w, float Angle_w)
{
   
    Target_Angle_w = fmod(Target_Angle_w, 360.0f);
    if (Target_Angle_w < 0) Target_Angle_w += 360.0f;
		Angle_w = fmodf(Angle_w, 360.0f);
    if (Angle_w < 0) Angle_w += 360.0f;
   
    float err_w = Target_Angle_w - Angle_w;
   
    if (err_w > 180.0f) {
        err_w -= 360.0f;
    } else if (err_w < -180.0f) {
        err_w += 360.0f;
    }
    static float last_err_w, integral_w;
    float out_w = 1.0*err_w +0*(err_w - last_err_w )+ 0.008*integral_w;
    last_err_w = err_w;
    integral_w += err_w;
    integral_w = func_limit(integral_w, 2000); // 积分限幅
    return out_w;
}

//  角度环（外环pid）
float Kp_A_new=0.8,Kd_A_new=0,Ki_A_new=0; // 20/95/0  Kp_A = 1.25,Kd_A = 8       Kp_A=0.8,Kd_A=6.4,Ki_A=0
float Turn_w_TLY_2(float Target_Angle, float Angle)
{	
	float err = Target_Angle - Angle;
	static float last_err, integral;
	float out = Kp_A_new*err +Kd_A_new*(err - last_err) + Ki_A_new*integral;
	last_err = err;
	integral += err;
	integral = func_limit(integral,200); //积分限幅
    return out;
}



