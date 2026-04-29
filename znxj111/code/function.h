#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include "zf_common_headfile.h"

/*********** 宏变量 ***********/
#define JUMP_POINTS 1

/******************* 函数 *******************/
	/*** 总循迹函数 ***/
void Tracking();
	
	/*** 搜线辅助函数 ***/
uint8 find_extreme_value(uint8 *array_value,uint8 num0,uint8 num1,uint8 model);

	/*** 十字辅助函数 ***/
void Left_Add_Line(int x1,int y1,int x2,int y2);
void Right_Add_Line(int x1,int y1,int x2,int y2);
void Find_Down_Point(int start,int end);
void Find_Up_Point(int start,int end);
void Lengthen_Left_Boundry(int start,int end);
void Lengthen_Right_Boundry(int start,int end);

	/*** 圆环辅助函数 ***/
int Find_Right_Island_Out(int start_x,int start_y);

int Find_Left_Down_Points(int start,int end);
int Find_Left_Up_Points(int start,int end);
int Find_Right_Down_Points(int start,int end);
int Find_Right_Up_Points(int start,int end);

int Continuity_Change_Right(int start,int end);
int Continuity_Change_Left(int start,int end);

int Monotonicity_Change_Left(int start,int end);
int Monotonicity_Change_Right(int start,int end);

void K_Add_Boundry_Left(float k,int startX,int startY,int endY);
void K_Add_Boundry_Right(float k,int startX,int startY,int endY);

void K_Draw_Line(float k, int startX, int startY,int endY);

void Draw_Line(int startX, int startY, int endX, int endY);

void Image_Flag_Show(uint8 MT9V03XW,uint8(*InImg)[MT9V03XW],uint8 image_flag);

#endif
