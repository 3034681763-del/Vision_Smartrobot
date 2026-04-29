#include "new_island.h"

int Left_Island_Flag_new  = 0;
int Right_Island_Flag_new = 0;
int Left_Up_TurningPoint_new [2];
int Right_Up_TurningPoint_new[2];

Island_Angle Angle;

//---------------------------------------------------------------
//  新环岛检测
//	走Ω
//	MT9V03X_W = 180, MT9V03X_H = 80
//  Island_Detect_new()
//----------------------------------------------------------------
void Island_Detect_new()
{
	static int Left_Down_Turn[2] ={0};       //四个拐点的坐标储存，{0} -> y,{1} -> x
	static int Right_Down_Turn[2]={0};
	static int Island_state_3_up[2]={0};	//状态3时即将进入环岛，左右上边角点，{0} -> y,{1} -> x
	
	
	int Monotonicity_Change_Line[2];        //单调性改变点的坐标，{0} -> y,{1} -> x
	int Monotonicity_Change_Left_Flag = 0;  //单调没转折为0
	int Monotonicity_Change_Right_Flag = 0;
	int Continuity_Change_Right_Flag=0; 	//连续为0
    int Continuity_Change_Left_Flag=0; 
	
	static float k_island = 0;
	
	static int cntt = 0;
	
	if(Control_Mode != 0)
	{
		return;
	}

	/*
		1.单调性改变检测:	
			return	改变的那一行
			检测方向:	从图像下方向上检测,由近到远
			检测方法:	找出边线凸出来的点,凸出来点的x坐标是上下5行中最大的(左侧) 或者 最小的(右侧)
			未检测到时返回值:	0

		2.连续性检测:
			return	改变的那一行
			检测方向:	从图像下方向上检测,由近到远
			检测方法:	判断相邻点x坐标差距,上边比下边的点差 > 5(需要根据实际情况调整)
			未检测到时返回值:	0
	*/
	Monotonicity_Change_Left_Flag  = Monotonicity_Change_Left(START_ROW-10,10);
	Monotonicity_Change_Right_Flag = Monotonicity_Change_Right(START_ROW-10,10);
	Continuity_Change_Right_Flag   = Continuity_Change_Left(START_ROW-5,10);
	Continuity_Change_Left_Flag    = Continuity_Change_Right(START_ROW-5,10);
	
	/* 圆环判断开始 */
	if(Cross_Flag == 0 && Island_State == 0)	//不是十字，也没有在圆环内
	{
		/*
			首先判断连续性,圆环存在一边连续一边不连续
		*/
		Continuity_Change_Left_Flag = Continuity_Change_Left(START_ROW - 5, 10);
		Continuity_Change_Right_Flag = Continuity_Change_Right(START_ROW - 5, 10);

		/*
			左圆环判断开始
		*/
		if(Left_Island_Flag_new == 0)
		{
			/*
				State_1
				判断条件:
					1.右侧单调无转折	Monotonicity_Change_Right_Flag = 0
					2.左侧不连续		Continuity_Change_Left_Flag   != 0
					3.右侧连续			Continuity_Change_Right_Flag   = 0
					4.左侧丢线很多但不能全丢	10 <= Left_lose_Time <= 50
					5.右边丢线很少		Right_lose_Time <= 10
					6.双边丢线少		Both_lose_time	<= 10
					7.最长白列很长		Search_Stop_Line <= MT9V03X_H*0.1 (我觉得可以改一改大小)
					8.左边界起点靠下	Boundry_Start_Left  >= MT9V03X_H - 20
					9.右边界起点靠下	Boundry_Start_Right >= MT9V03X_H - 20
			*/
			if (Monotonicity_Change_Right_Flag == 0
				&& Continuity_Change_Left_Flag != 0
				&& Continuity_Change_Right_Flag == 0
				&& Left_lose_Time >= 10  
				&& Left_lose_Time <= 30  
				&& Right_lose_Time <= 10 
//				&& Search_Stop_Line <= MT9V03X_H * 0.1  
//				&& Boundry_Start_Left >= MT9V03X_H - 20 
//				&& Boundry_Start_Right >= MT9V03X_H - 20
//				&& Both_lose_time <= 10
			) 
			{
				/*
					左下角点检测:
						return	角点所在行
						检测方向:	从图像下方向上检测,由近到远
						检测方法:	此点与靠下的三个点间，相邻两点x相差很小 (下方连续)
									与靠上三个点的差值大于一个梯度的限度	(上方不连续)
						未检测到时返回值:	0
				*/
				
//				printf("In");
				Left_Down_Turn[0] = Find_Left_Down_Points(START_ROW, 20);
				//printf("Left_Down_Turn[0] = %d\n",Left_Down_Turn[0]);
				
				if (Left_Down_Turn[0] >= 24
					&& reference_col <= (MT9V03X_W/2 + 22)
					&& reference_col >= (MT9V03X_W/2 - 22))
				{
					Island_State = 1;
					Left_Island_Flag_new = 1;
				}
				//误判清零
				else
				{
					//printf("In");
					Island_State = 0;
					Left_Island_Flag_new = 0;
				}
			}

		}

		/*
			右圆环判断开始
		*/
		if (0 == Right_Island_Flag_new)
		{
			/*
				State_1
				判断条件:
					1.左侧单调无转折	Monotonicity_Change_Left_Flag	= 0
					2.右侧不连续		Continuity_Change_Right_Flag   != 0
					3.左侧连续			Continuity_Change_Left_Flag     = 0
					4.右侧丢线很多但不能全丢	10 <= Right_lose_Time <= 50
					5.左边丢线很少		Left_lose_Time  <= 10
					6.双边丢线少		Both_lose_time	<= 10
					7.最长白列很长		Search_Stop_Line <= MT9V03X_H*0.1 (我觉得可以改一改大小)
					8.左边界起点靠下	Boundry_Start_Left  >= MT9V03X_H - 20
					9.右边界起点靠下	Boundry_Start_Right >= MT9V03X_H - 20
			*/
			if (Monotonicity_Change_Left_Flag == 0
				&& Continuity_Change_Left_Flag == 0
				&& Continuity_Change_Right_Flag != 0
				&& Right_lose_Time >= 10  
				&& Right_lose_Time <= 50  
				&& Left_lose_Time <= 10   
//				&& Search_Stop_Line <= MT9V03X_H * 0.1  
//				&& Boundry_Start_Left >= MT9V03X_H - 20 
//				&& Boundry_Start_Right >= MT9V03X_H - 20
//				&& Both_lose_time <= 10
			)  
			{
				/*
					右下角点检测:
						return	角点所在行
						检测方向:	从图像下方向上检测,由近到远
						检测方法:	此点与靠下的三个点间，相邻两点x相差很小 (下方连续)
									与靠上三个点的差值大于一个梯度的限度	(上方不连续)
						未检测到时返回值:	0
				*/
				Right_Down_Turn[0] = Find_Right_Down_Points(START_ROW, 20);
				
				cntt++;
				
//				printf("right_down_turn[0] = %d\n",Right_Down_Turn[0]);
//				printf("right_down_turn-----finish\n");
				//printf("进入右环开始判断次数: %d\n",cntt);
				
				if (Right_Down_Turn[0] >= 24
					&& reference_col <= (MT9V03X_W/2 + 22)
					&& reference_col >= (MT9V03X_W/2 - 22))
				{
					Island_State = 1;
					Right_Island_Flag_new = 1;
				}
				else
				{
					Island_State = 0;
					Right_Island_Flag_new = 0;
				}
			}
		}
	}

		/*
			关键参数显示
		*/
//		ips200_show_int(0, 6 * 16, Left_Island_Flag_new, 2);	
//		ips200_show_int(0, 7 * 16, Right_Island_Flag_new, 2);
//		ips200_show_int(0, 8 * 16, Island_State, 2);		
//	
//		ips200_show_int(0, 10 * 16, Continuity_Change_Left_Flag, 3);	//左连续性
//		ips200_show_int(0, 11 * 16, Continuity_Change_Right_Flag, 3);	//右连续性
//		ips200_show_int(0, 12 * 16, Monotonicity_Change_Left_Flag, 3);	//左单调性
//		ips200_show_int(0, 13 * 16, Monotonicity_Change_Right_Flag, 3); //右单调性
//	
//		ips200_show_int(30, 6 * 16, Left_lose_Time, 3);		//左丢线数			 
//		ips200_show_int(30, 7 * 16, Right_lose_Time, 3);	//右丢线数
//		ips200_show_int(30, 8 * 16, Both_lose_time, 3);		//右丢线数
//	
//		ips200_show_int(30, 10 * 16, Search_Stop_Line, 3);		//最长白列截至行
//		ips200_show_int(30, 11 * 16, reference_col, 3);			//白列位置
//		ips200_show_int(30, 12 * 16, Boundry_Start_Left, 3);	//左边界起点		 
//		ips200_show_int(30, 13 * 16, Boundry_Start_Right, 3);	//右边界起点
//	
//		ips200_show_float(60, 6 * 16, k_island, 3, 3);	//入环拉线斜率	
//		ips200_show_float(60, 7 * 16, Err_Sum()*0.27,3,3);		
//		ips200_show_int(60, 8 * 16, Monotonicity_Change_Line[0], 3);	
//		ips200_show_int(60, 9 * 16, Monotonicity_Change_Line[1], 3);	
		
		
		
		/*
			下角点还在,正式开始判断圆环状态
		*/

		/*
			左圆环状态判断
			左下拐点还没丢
		*/
		if (Left_Island_Flag_new == 1)
		{
			/*
				State2
				判断条件:
					1.圆环状态为1	Island_State = 1
					2.左下起始点靠上 (角点消失,左下丢线)	Left_Down_Turn[0] < 45
			*/
			if(Island_State == 1)
			{
				/*
					State1	补线策略
					1.找到圆环单调性突变点
					2.连接拐点与单调突变点
				*/
				Monotonicity_Change_Line[0] = Monotonicity_Change_Left(30, 5);	//y
				Monotonicity_Change_Line[1] = left_edge_line[Monotonicity_Change_Line[0]];
				
//////////////////////////////////				ips200_show_int(60, 8 * 16, Monotonicity_Change_Line[0], 3);	
//////////////////////////////////				ips200_show_int(60, 9 * 16, Monotonicity_Change_Line[1], 3);
				/*
					左补线函数:
						void
						补线方向:	从上向下补线
						补线方法:	两点式
				*/
				if(Monotonicity_Change_Line[0] != 0
					&& Monotonicity_Change_Line[1] <= 160
					&& Monotonicity_Change_Line[1] >= 20)
				{
					Left_Add_Line((int)(Monotonicity_Change_Line[1] * 0.13), START_ROW, Monotonicity_Change_Line[1], Monotonicity_Change_Line[0]);
				}
				
				
				/*
					防止误判的条件:	(大S弯,急弯误判)
						1.Left_Down_Turn[0] = 0		找不到下角点
						2.Monotonicity_Change_Line[1] >= 2*(SEARCH_BORDER_R)/3	单调点过于靠右
						3.Search_Stop_Line >= 20	最长白列较短
						
						4.Right_lose_Time <= 5	右侧连续
						5.左侧断的多
				*/
				Left_Down_Turn[0] = Find_Left_Down_Points(START_ROW, 20);
				
				if(Left_Down_Turn[0] == 0
					&& ((Right_lose_Time <= 5 || Search_Stop_Line >= 30)|| (Left_lose_Time  <= 5 || Search_Stop_Line >= 30))
					&& (reference_col >= (2*(SEARCH_BORDER_R)/3) || reference_col <= (1*(SEARCH_BORDER_R)/3))
					&& Monotonicity_Change_Line[1] >= 2*(SEARCH_BORDER_R)/3)
				{
					Island_State = 0;
					Left_Island_Flag_new = 0;
				}	
				else if(Search_Stop_Line >= 9)
				{
					Island_State = 0;
					Left_Island_Flag_new = 0;
				}
				
//				if(Left_lose_Time >= 20
//					&& Right_lose_Time <= 5
//					&& Search_Stop_Line >= 20)
//				{
//					Island_State = 0;
//					Left_Island_Flag_new = 0;
//				}
				
				if(Boundry_Start_Left <= 54)
				{
					Island_State = 2;
					Angle.T_M1 = T_M;	//记录当前角度
				}


			}
			/*
				State3
				判断条件:
					1.圆环状态为2	Island_State = 2
					2.角度变化		Turn_Angle >= 50
			*/
			else if(Island_State == 2)
			{	
				if(k_island != 0)
				{
//					printf("i m out\n");
					
					Angle.T_M2 = T_M;
					Angle.Turn_Angle = abs(Angle.T_M2 - Angle.T_M1);
					
					K_Draw_Line(k_island,SEARCH_BORDER_R,START_ROW,5);
					/***** 重新进行搜线 *****/
					search_reference_col(bin_image[0]);
					New_Search_Line(bin_image);
				}
				else if(k_island == 0)
				{
//					printf("i m in\n");
					Monotonicity_Change_Line[0] = Monotonicity_Change_Left(30, 5);	//y
					Monotonicity_Change_Line[1] = left_edge_line[Monotonicity_Change_Line[0]];
					if(k_island == 0
						&& Monotonicity_Change_Line[0] != 0
						&& Monotonicity_Change_Line[1] <= 140
						&& Monotonicity_Change_Line[1] >= 40)
					{
						Island_state_3_up[0] = Monotonicity_Change_Line[0] + 30;
						Island_state_3_up[1] = Monotonicity_Change_Line[1];
						
						k_island = (float)((float)(MT9V03X_H - Island_state_3_up[0])/(float)(MT9V03X_W-1 - Island_state_3_up[1]));
						//k_island = (float)((float)(MT9V03X_H - Island_state_3_up[0])/(float)(Island_state_3_up[1]));
//						printf("k:%f\n",k_island);
						K_Draw_Line(k_island,SEARCH_BORDER_R,START_ROW,5);
						/***** 刷新边界 *****/
						search_reference_col(bin_image[0]);
						New_Search_Line(bin_image);
					}
					else
					{
						Island_State = 0;
						Left_Island_Flag_new = 0;
					}
				}
				
				if(Island_State == 2
					&& Angle.Turn_Angle >= 42)
				{
					Island_State = 3;
//					k_island = 0;
					Angle.T_M3 = T_M;
					/***** 刷新边界 *****/
					search_reference_col(bin_image[0]);
					New_Search_Line(bin_image);
				}
				
			}
			/*
				State4
				判断条件:
					1.圆环状态为3	Island_State = 3
					2.角度变化		Turn_Angle >= 200
			*/
			else if(Island_State == 3)
			{
				Angle.T_M4 = T_M;
				Angle.Turn_Angle = abs(Angle.T_M4 - Angle.T_M3);
				
				if(Angle.Turn_Angle >= 90)
				{
					Monotonicity_Change_Line[0] = Monotonicity_Change_Left(80,10);
					Monotonicity_Change_Line[1] = left_edge_line[Monotonicity_Change_Line[0]];
//					
					if(Island_State == 3
						&& Monotonicity_Change_Line[0] >= 68
						&& Monotonicity_Change_Line[0] <= 80
						&& Monotonicity_Change_Line[1] >= 5
						//&& Boundry_Start_Right <= 45
						//&& left_edge_line[START_ROW] == SEARCH_BORDER_L
						)
					{
						K_Draw_Line(k_island,SEARCH_BORDER_R,START_ROW,5);
						/***** 刷新边界 *****/
						search_reference_col(bin_image[0]);
						New_Search_Line(bin_image);
						
						Angle.T_M5 = T_M;
						Island_State = 4;
					}
				}
				
			}
			/*
				出环判断
			*/
			else if(Island_State == 4)
			{
//				Position_Control();
				
				K_Draw_Line(k_island,SEARCH_BORDER_R,START_ROW,5);
				/***** 刷新边界 *****/
				search_reference_col(bin_image[0]);
				New_Search_Line(bin_image);
				
				Angle.T_M6 = T_M;
				Angle.Turn_Angle = abs(Angle.T_M6 - Angle.T_M5);
				
				if(Angle.Turn_Angle >= 31)
				{
					Island_State = 0;
					k_island = 0;
					Left_Island_Flag_new = 0;
					/***** 刷新边界 *****/
					search_reference_col(bin_image[0]);
					New_Search_Line(bin_image);
				}
				
			}
			
		}
		
		/*
			右环检测
			右下拐点还没丢
		*/
		if(Right_Island_Flag_new == 1)
		{
			/*
				State2
				判断条件:
				1.圆环状态为1	Island_State = 1
				2.右下起始点靠上 (角点消失,右下丢线)	Right_Down_Turn[0] < 45
			*/
			if(Island_State == 1)
			{
				/*
					State1	补线策略
					1.找到圆环单调性突变点
					2.连接拐点与单调突变点
				*/
				Monotonicity_Change_Line[0] = Monotonicity_Change_Right(30, 5);	//y
				Monotonicity_Change_Line[1] = right_edge_line[Monotonicity_Change_Line[0]];
				
//////////////////////////////////////////////////				ips200_show_int(60, 8 * 16, Monotonicity_Change_Line[0], 3);	
//////////////////////////////////////////////////				ips200_show_int(60, 9 * 16, Monotonicity_Change_Line[1], 3);
				
				if(Monotonicity_Change_Line[0] != 0
					&& Monotonicity_Change_Line[1] >= 20
					&& Monotonicity_Change_Line[1] <= 160
				)
				{
					Right_Add_Line((int)(SEARCH_BORDER_R - Monotonicity_Change_Line[1] * 0.17), START_ROW, Monotonicity_Change_Line[1], Monotonicity_Change_Line[0]);
				}
				
				
				/*
					防误判
				*/
				if(Search_Stop_Line >= 9)
				{
					Island_State = 0;
					Right_Island_Flag_new = 0;
//					printf("");
				}
				
				//Right_Down_Turn[0] = Find_Right_Down_Points(START_ROW, 20);
				
				/*** 状态2进环使用 ***/
//				if(Right_Down_Turn[0] != 0)
//				{
//					Island_state_3_up[0] = Find_Right_Island_Out(Right_Down_Turn[0],Monotonicity_Change_Line[0]);
//					Island_state_3_up[1] = right_edge_line[1];
//				}
				
			
				if(Island_State == 1
					&& Boundry_Start_Right  <= 48)
				{
					Island_State = 2;
					Angle.T_M1 = T_M;	//记录当前角度
				}
				
			}
			
			/*
				State3
				判断条件:
					1.圆环状态为2	Island_State = 2
					2.角度变化		Turn_Angle >= 50
			*/
			else if(Island_State == 2)
			{
				if(k_island != 0)
				{
					//printf("i m out\n");
					
					Angle.T_M2 = T_M;
					Angle.Turn_Angle = abs(Angle.T_M2 - Angle.T_M1);
					
					K_Draw_Line(-k_island,SEARCH_BORDER_L,START_ROW,5);
					/***** 重新进行搜线 *****/
					search_reference_col(bin_image[0]);
					New_Search_Line(bin_image);					
				}
				else if(k_island == 0)
				{
					Monotonicity_Change_Line[0] = Monotonicity_Change_Right(30, 5);	//y
					Monotonicity_Change_Line[1] = right_edge_line[Monotonicity_Change_Line[0]];
					
					if(k_island == 0
						&& Monotonicity_Change_Line[0] != 0
						&& Monotonicity_Change_Line[1] >= 40
						&& Monotonicity_Change_Line[1] <= 140)
					{
						Island_state_3_up[0] = Monotonicity_Change_Line[0] + 20;
						Island_state_3_up[1] = Monotonicity_Change_Line[1];
						
						k_island = (float)((float)(MT9V03X_H - Island_state_3_up[0])/(float)(Island_state_3_up[1]));
						//k_island = 0.532;
						//printf("k:%f\n",k_island);
						K_Draw_Line(-k_island,SEARCH_BORDER_L,START_ROW,5);
						/***** 刷新边界 *****/
						search_reference_col(bin_image[0]);
						New_Search_Line(bin_image);
					}
					else
					{
						Island_State = 0;
						Right_Island_Flag_new = 0;
					}
				}
				
				if(Island_State == 2
					&& Angle.Turn_Angle >= 34)
				{
					Island_State = 3;
//					k_island = 0;
					Angle.T_M3 = T_M;
					/***** 刷新边界 *****/
					search_reference_col(bin_image[0]);
					New_Search_Line(bin_image);
				}
				
			}
			
			/*
				State4
				判断条件:
					1.圆环状态为3	Island_State = 3
					2.角度变化		Turn_Angle >= 120
			*/
			else if(Island_State == 3)
			{
				Angle.T_M4 = T_M;
				Angle.Turn_Angle = abs(Angle.T_M4 - Angle.T_M3);
				
				if(Angle.Turn_Angle >= 90)
				{
					Monotonicity_Change_Right_Flag = Monotonicity_Change_Right(START_ROW-10,10);
					Monotonicity_Change_Line[0] = Monotonicity_Change_Right(80,10);
					Monotonicity_Change_Line[1] = right_edge_line[Monotonicity_Change_Line[0]];
//					
					if(Island_State == 3
						&& Monotonicity_Change_Line[0] >= 63
						&& Monotonicity_Change_Line[0] <= 80
						&& Monotonicity_Change_Line[1] >= 5
						//&& Monotonicity_Change_Right_Flag != 0
						//&& Boundry_Start_Right <= 45
						//&& right_edge_line[START_ROW] == SEARCH_BORDER_R
						)
					{
						Island_state_3_up[0] = Find_Right_Island_Out(Monotonicity_Change_Line[1],Monotonicity_Change_Line[0]);
						Island_state_3_up[1] = Monotonicity_Change_Line[1];
						
						k_island = (float)((float)(MT9V03X_H - Island_state_3_up[0])/(float)(Island_state_3_up[1]));
						
						K_Draw_Line(k_island,SEARCH_BORDER_L,START_ROW,5);
						/***** 刷新边界 *****/
						search_reference_col(bin_image[0]);
						New_Search_Line(bin_image);
						
						Angle.T_M5 = T_M;
						Island_State = 4;
					}
				}
			}
			
			/*
				出环判断
			*/
			else if(Island_State == 4)
			{
//				Position_Control();
				
				K_Draw_Line(-k_island,SEARCH_BORDER_L,START_ROW,5);
				/***** 刷新边界 *****/
				search_reference_col(bin_image[0]);
				New_Search_Line(bin_image);
				
				Angle.T_M6 = T_M;
				Angle.Turn_Angle = abs(Angle.T_M6 - Angle.T_M5);
				
				if(Angle.Turn_Angle >= 31)
				{
					Island_State = 0;
					k_island = 0;
					Right_Island_Flag_new = 0;
					/***** 刷新边界 *****/
					search_reference_col(bin_image[0]);
					New_Search_Line(bin_image);
				}
				
			}
			
		}
	
}








