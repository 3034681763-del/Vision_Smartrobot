#ifndef __SEARCH_LINE_H__
#define __SEARCH_LINE_H__
#include "zf_common_headfile.h"

/***************** 宏变量 *****************/
#define SEARCH_IMAGE_W 	MT9V03X_W
#define SEARCH_IMAGE_H	MT9V03X_H
#define CONTRASTOFFSET 	1
#define STOP_ROW       	1
#define START_ROW 	   	MT9V03X_H-1
#define SEARCH_BORDER_L 1
#define SEARCH_BORDER_R MT9V03X_W-1
#define SEARCH_RANGE    10

#define WHITE_POINTS_BREAK	22
#define WHITE_RANGE			52
#define WHITE_STOP_LINE		5

/******************* 变量 *******************/
	/************************************ 二值化图像 ************************************/
extern uint8 original_image[MT9V03X_H][MT9V03X_W];
extern uint8 bin_image[MT9V03X_H][MT9V03X_W];
extern uint8 image_thereshold;

	/************************************ 搜线 ************************************/
extern uint8 reference_col;		//动态参考列 --> x
extern uint8 remote_distance[SEARCH_IMAGE_W];	//最长白列数组 --> y
extern uint8 Search_Stop_Line;	//截止行 --> y

extern uint8 left_edge_line[SEARCH_IMAGE_H]; //左边界
extern uint8 right_edge_line[SEARCH_IMAGE_H];//右边界
extern uint8 middle_line[SEARCH_IMAGE_H];	 //中线
extern uint8 Boundry_Start_Left ; //第一个非丢线点
extern uint8 Boundry_Start_Right;

extern uint8 Left_lose_flag[SEARCH_IMAGE_H] ;
extern uint8 Right_lose_flag[SEARCH_IMAGE_H];
extern uint8 Left_lose_Time ;
extern uint8 Right_lose_Time;
extern uint8 Both_lose_time ;

extern uint8 track_width[SEARCH_IMAGE_H];
	
	/************************************ 十字检测 ************************************/
extern uint8 Right_Down_Find;
extern uint8 Left_Down_Find;
extern uint8 Right_Up_Find;
extern uint8 Left_Up_Find;

	/************************************ 十字检测 ************************************/
extern uint8 Cross_Flag;
extern uint8 Island_State;
extern uint8 Zebra_Flag;
extern uint8 Stop_Flag;
extern uint8_t Start_Count;      
extern uint8_t Last_Zebra_State; 
extern uint8 Stop_State;

/******************* 函数 *******************/
	/********* 二值化图像 *********/
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row);
void get_image(uint8(*image)[MT9V03X_W]);
void turn_to_bin(void);

	/********* 搜线 *********/
void search_reference_col(const uint8 *image);
void New_Search_Line(const uint8 (*image)[MT9V03X_W]);
void Show_Boundry();

	/********* 十字 *********/
void Cross_Detect();	

	/********* 误差 *********/
float Err_Sum();
float Err_Sum_INisland_L();
float Err_Sum_INisland_R();

	/********* 停车 *********/
void Zebra_Stop();

#endif
