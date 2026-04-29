#include "zf_common_headfile.h"
#include <math.h>

#define ENCODER_1                   (QTIMER2_ENCODER2)
#define ENCODER_1_A                 (QTIMER2_ENCODER2_CH1_C5) //right
#define ENCODER_1_B                 (QTIMER2_ENCODER2_CH2_C25)
#define ENCODER_3                   (QTIMER2_ENCODER1)
#define ENCODER_3_A                 (QTIMER2_ENCODER1_CH1_C3)//left
#define ENCODER_3_B                 (QTIMER2_ENCODER1_CH2_C4)
#define ENCODER_4                   (QTIMER1_ENCODER1)
#define ENCODER_4_A                 (QTIMER1_ENCODER1_CH1_C0)
#define ENCODER_4_B                 (QTIMER1_ENCODER1_CH2_C1)
#define speed_size 20
int16 encoder_data [4];
int16 encoder[4];
int16 encoder_data_last[4]={0};
float aim_speed        =0;  //目标速度  18   55
float aim2_speed        =0;  //目标速度  18   55
float target_slope  = 0;  //目标斜率
float history_speed_left[20];
float history_speed_right[20];
float history_speed_behind[20];
float speed_behind=0;
float speed_right=0;
float speed_left=0;
float distance;
void my_encoder_init(void)
{

	   encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // 初始化编码器模块与引脚 正交解码编码器模式

     encoder_quad_init(ENCODER_3, ENCODER_3_A, ENCODER_3_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
     encoder_quad_init(ENCODER_4, ENCODER_4_A, ENCODER_4_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
	
}


void pit_handler_0 (void)
{
   encoder[0] = encoder_get_count(ENCODER_1);
   encoder[1] = encoder_get_count(ENCODER_3);
	 encoder[2] = encoder_get_count(ENCODER_4);
	
	

	 encoder_data[0]=smooth_speed_realtime(encoder[0], history_speed_right, speed_size);
	 encoder_data[1]=smooth_speed_realtime(encoder[1], history_speed_left, speed_size);
	 encoder_data[2]=smooth_speed_realtime(encoder[2], history_speed_behind, speed_size);
	
	 speed_behind+=encoder_data[2]*0.002;
	 speed_right+=encoder_data[0]*0.002;
	 speed_left+=encoder_data[1]*0.002;
 //  encoder_data[2] = encoder_get_count(ENCODER_3);
    encoder_clear_count(ENCODER_1);
	  encoder_clear_count(ENCODER_3);
	  encoder_clear_count(ENCODER_4);
	
}







 