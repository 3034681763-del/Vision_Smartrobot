#include "zf_common_headfile.h"
#include <math.h>

//若更换板子，需要改的地方：1.电机信号引脚； 2编码器的引脚， 同时不要忘了编码器的模块
//另外若更换小车， 则motor.c中pwm设置高低电平
#define MOTOR1_DIR               (C10)				
#define MOTOR1_PWM               (PWM2_MODULE2_CHB_C11)
#define MOTOR2_DIR               (D2)
#define MOTOR2_PWM               (PWM2_MODULE3_CHB_D3)//C8
#define MOTOR3_DIR               (C6)                                                      
#define MOTOR3_PWM               (PWM2_MODULE1_CHA_C8)

float Target_Speed_C;
float Target_Speed_A=55;//55
float Target_Speed_B ;//55

int16 Target_Speed_cs=0;

void IncPIDInit(PID *sptr)
{
    sptr->SumError=0;
    sptr->LastError=0;
    sptr->LLastError=0;

    sptr->Kp=0;
    sptr->Ki=0;
    sptr->Kd=0;
}



void my_motor_init()
{
    gpio_init(MOTOR1_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR1_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0
    
    gpio_init(MOTOR2_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR2_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

    gpio_init(MOTOR3_DIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                            // GPIO 初始化为输出 默认上拉输出高
    pwm_init(MOTOR3_PWM, 17000, 0);                                                  // PWM 通道初始化频率 17KHz 占空比初始为 0

}

void motor_set_duty(int16 left_PWM,int16 right_PWM,int16 behind_PWM)
{
	//ips114_show_int(40,40,duty,5);
	left_PWM = func_limit(left_PWM, 6000);//6000
	right_PWM = func_limit(right_PWM, 6000);
	behind_PWM = func_limit(behind_PWM, 6000);
    if(left_PWM >= 0)   //正转
    {
      gpio_set_level(MOTOR1_DIR,GPIO_HIGH); 
			pwm_set_duty(MOTOR1_PWM, (uint32_t)left_PWM);
	

    }
		
    else           //反转
    {
      gpio_set_level(MOTOR1_DIR, GPIO_LOW); 
			pwm_set_duty(MOTOR1_PWM, (uint32_t)-left_PWM);
    }   
		
		 if(right_PWM >= 0)   //正转
    {
			
			gpio_set_level(MOTOR2_DIR, GPIO_HIGH);
			pwm_set_duty(MOTOR2_PWM, (uint32_t)right_PWM);
    }
    else           //反转
    {
 
			gpio_set_level(MOTOR2_DIR,GPIO_LOW) ; 
			pwm_set_duty(MOTOR2_PWM, (uint32_t)-right_PWM);

    }   
		
		 if(behind_PWM >= 0)   //正转
    {
			
			
			gpio_set_level(MOTOR3_DIR, GPIO_HIGH); 
			pwm_set_duty(MOTOR3_PWM, (uint32_t)behind_PWM);

    }
    else           //反转
    {
 
			gpio_set_level(MOTOR3_DIR, GPIO_LOW); 
			pwm_set_duty(MOTOR3_PWM, (uint32_t)-behind_PWM);

    }   
		
		
		
}


/************************************************
函数名：IncPIDCalc(PID *sptr,int16 Setpoint,int16 Turepoint)
功  能：增量式PID控制
************************************************/
int16 IncPIDCalc(PID *sptr,int16 Setpoint,int16 Turepoint)
{
    int16 iError,iIncpid;
    //当前误差
    iError=Setpoint-Turepoint;      //偏差

    iIncpid=sptr->Kp*(iError-sptr->LastError)
             +sptr->Ki*iError;
//    //储存误差，用于下次计算
    if(iIncpid>=5000)   //每次输出增量限幅
      {
          iIncpid=5000;
      }
    if(iIncpid<=-5000)
      {
          iIncpid=-5000;
      }
            sptr->LastError=iError;
      return  iIncpid;
}
/************************************************
函数名：LocP_DCalc(PID *sptr,int16 Setpoint,int16 Turepoint)
功  能：位置式PID控制
************************************************/
int16 LocP_DCalc(PID*sptr,int16 Setpoint,int16 Turepoint)
{
    int16 iError,dError;
    int16 output;

    iError=Setpoint-Turepoint;  //偏差
    sptr->SumError+=iError;            //积分(采样时间很短时，用一阶差分代替一阶微分，用累加代替积分)
    dError=(int16)(iError-(sptr->LastError));     //微分
  //  sptr->LastError=iError;
    if(sptr->SumError>2000) sptr->SumError=2000;   //积分限幅
    if(sptr->SumError<-2000) sptr->SumError=-2000;
	
    output=(int16)(sptr->Kp*iError  //比例项
       //  +(sptr->Ki*sptr->SumError)//积分项
          +sptr->Kd*dError);        //微分项
	   sptr->LastError=iError;
    return output;
}

void speed()
{
   static int t=0; 
   t++;
   Target_Speed_cs=30+sin(0.2*0.3*t)*20;

}
void speed_1()
{
	 target_speed=20; 
   static int t=0; 
   t++;
   if(t%4==1)target_speed=30; 
	 if(t*4==2)target_speed=40; 
	 if(t%4==3)target_speed=50;
	 if(t%4==0)target_speed=60;

}



