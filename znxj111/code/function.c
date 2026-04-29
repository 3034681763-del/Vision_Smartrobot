#include "function.h"

//---------------------------------------------------------------
//  总循迹控制函数
//  Tracking()
//---------------------------------------------------------------
void Tracking()
{
	if(Stop_Flag == 1)
	{
		//Car_Stop();
		return;
	}
	
	if(mt9v03x_finish_flag)
	{
		get_image(mt9v03x_image);
		turn_to_bin();
		mt9v03x_finish_flag = 0;
	}	
	search_reference_col(bin_image[0]);
	New_Search_Line(bin_image);
	
	Cross_Detect();
	Island_Detect_new();	
	
	/*** 搜线 ***/
//	ips200_show_int(0, 6 * 16, reference_col, 3);
//	ips200_show_int(0, 8 * 16, Left_lose_Time, 3);
//	ips200_show_int(0, 9 * 16, Right_lose_Time, 3);
//	ips200_show_int(0, 10 * 16, Both_lose_time, 3);
	
	/*** 十字 ***/
//	ips200_show_int(40, 6 * 16, Left_Up_Find, 3);
//	ips200_show_int(40, 7 * 16, Right_Up_Find, 3);
//	ips200_show_int(40, 8 * 16, Left_Down_Find, 3);
//	ips200_show_int(40, 9 * 16, Right_Down_Find, 3);
	
//	ips200_show_float(60,7 * 16,Err_Sum()*0.27,3,3);
	
//	Show_Boundry();
//  ips200_show_gray_image(0, 0, bin_image[0], MT9V03X_W, MT9V03X_H, MT9V03X_W, MT9V03X_H, 0);
}

/**************************************************************************************************************************/
/****************************************************** 搜线 **************************************************************/
/**************************************************************************************************************************/
//---------------------------------------------------------------
//  遍历数组最大值或最小值
//  find_exreme_value  数组指针
//  num0               起始范围
//  num1               终止范围
//  model              1 返回最大值  0 返回最小值
//  find_extreme_value(remote_distance,10,SEARCH_IMAGE_W - 10,0)
//----------------------------------------------------------------
uint8 find_extreme_value(uint8 *array_value,uint8 num0,uint8 num1,uint8 model)
{
	uint8 i = 0,temp = 0,temp1 = 0,temp2 = 0,value = 0;
	if(num0 > num1)
	{
		temp1 = num0 - num1;
		temp2 = num1;
		array_value += num0;
		value = *array_value;
		if(model)
		{
			for(i = 0;i <= temp1;i+= JUMP_POINTS)
			{
				temp = *(array_value - i);
				if(temp > value)
				{temp2 = num0 - i;value = temp;}
			}
		}
		else
		{
			for(i = 0;i <= temp1;i+= JUMP_POINTS)
			{
				temp = *(array_value - i);
				if(temp < value)
				{temp2 = num0 - i;value = temp;}
			}
		}
	}
	else
	{
		temp1 = num1 - num0; //区间长度
		temp2 = num0;        //区间开始
		array_value += num0; //从数组的第num0个数开始比较
		value = *array_value;
		if(model)
		{
			//找最大
			for(i = 0;i <= temp1;i++)
			{
				temp = *(array_value + i);
				if(temp > value)
				{temp2 = num0 + i;value = temp;}
			}
		}
		else
		{
			//找最小
			for(i = 0;i <= temp1;i++)
			{
				temp = *(array_value + i);
				if(temp < value)
				{temp2 = num0 + i;value = temp;}
			}
		}
	}
	return temp2;
}

/*---------------------------------------------------------------
	弯道处理
---------------------------------------------------------------*/
//void Curve_handling()
//{
//	
//}

/**************************************************************************************************************************/
/**************************************************** 十字检测 ************************************************************/
/**************************************************************************************************************************/
//---------------------------------------------------------------
//  左补线
//  Left_Add_Line(x1,y1,x2,y2)
//---------------------------------------------------------------
void Left_Add_Line(int x1,int y1,int x2,int y2)
{
	int i,MAXy,MAXx,a1,a2,b1,b2;
	int hx;
	
	//防止数组越界
	if(x1 >= SEARCH_BORDER_R)
	{
		x1 = SEARCH_BORDER_R;
	}
	else if(x1 <= SEARCH_BORDER_L)
	{
		x1 = 0;
	}
	if(y1 >= START_ROW)
	{
		y1 = START_ROW;
	}
	else if(y1 <= 0)
	{
		y1 = 0;
	}
	if(x2 >= SEARCH_BORDER_R)
	{
		x2 = SEARCH_BORDER_R;
	}
	else if(x2 <= SEARCH_BORDER_L)
	{
		x2 = 0;
	}
	if(y2 >= START_ROW)
	{
		y2 = START_ROW;
	}
	else if(y2 <= 0)
	{
		y2 = 0;
	}
	a1 = y1;
	a2 = y2;
	b1 = x1;
	b2 = x2;
	
	//找到靠上的坐标，从上向下补
	if(a1 > a2)
	{
		//y
		MAXy = a1;
		a1 = a2;
		a2 = MAXy;
		//x
		MAXx = b1;
		b1 = b2;
		b2 = MAXx;
	}
		
	for(i = a1;i < a2;i++)
	{
		hx = (i - y1)*(x2 - x1)/(y2 - y1) + x1;
		if(hx >= MT9V03X_W)
		{
			hx = MT9V03X_W;
		}
		else if(hx <= 0)
		{
			hx = 0;
		}
		left_edge_line[i] = hx;
		//printf("im L add line\n");
	}
}

//---------------------------------------------------------------
//  右补线
//  Right_Add_Line(x1,y1,x2,y2)
//---------------------------------------------------------------
void Right_Add_Line(int x1,int y1,int x2,int y2)
{
	int i,MAXy,MAXx,a1,a2,b1,b2;
	int hx;
	
	//防止数组越界
	if(x1 >= SEARCH_BORDER_R)
	{
		x1 = SEARCH_BORDER_R;
	}
	else if(x1 <= SEARCH_BORDER_L)
	{
		x1 = 0;
	}
	if(y1 >= START_ROW)
	{
		y1 = START_ROW;
	}
	else if(y1 <= 0)
	{
		y1 = 0;
	}
	if(x2 >= SEARCH_BORDER_R)
	{
		x2 = SEARCH_BORDER_R;
	}
	else if(x2 <= SEARCH_BORDER_L)
	{
		x2 = 0;
	}
	if(y2 >= START_ROW)
	{
		y2 = START_ROW;
	}
	else if(y2 <= 0)
	{
		y2 = 0;
	}
	a1 = y1;
	a2 = y2;
	b1 = x1;
	b2 = x2;
	
	if(a1 > a2)
	{
		//y
		MAXy = a1;
		a1 = a2;
		a2 = MAXy;
		//x
		MAXx = b1;
		b1 = b2;
		b2 = MAXx;
	}
	
	for(i = a1;i < a2;i++)
	{
		hx = (i - y1)*(x2 - x1)/(y2 - y1) + x1;
		if(hx >= MT9V03X_W)
		{
			hx = MT9V03X_W;
		}
		else if(hx <= 0)
		{
			hx = 0;
		}
		right_edge_line[i] = hx;
	}
}

//---------------------------------------------------------------
//  找下面两个拐点，供十字使用
//  Find_Down_Point(start,end)
//---------------------------------------------------------------
void Find_Down_Point(int start,int end)
{
	int i,t;
	
	Right_Down_Find = 0;
	Left_Down_Find = 0;
	
	if(start < end)
	{
		//保证start>end
		t = start;
		start = end;
		end = t;
	}
	if(start >= START_ROW-5)
	{
		//下5行数据不稳定
		start = START_ROW-5;
	}
	if(end <= Search_Stop_Line)
	{
		end = Search_Stop_Line;
	}
	if(end <= 5)
	{
		end = 5;
	}
	
	
	for(i = start;i > end;i--)
	{
		if(Left_Down_Find == 0 
			&& abs(left_edge_line[i]   - left_edge_line[i+1]) <= 5
			&& abs(left_edge_line[i+1] - left_edge_line[i+2]) <= 5
			&& abs(left_edge_line[i+2] - left_edge_line[i+3]) <= 5
			&& (left_edge_line[i] - left_edge_line[i-2]) >= 8
			&& (left_edge_line[i] - left_edge_line[i-3]) >= 10
			&& (left_edge_line[i] - left_edge_line[i-4]) >= 10)
		{
			Left_Down_Find = i;
			//printf("find L_D :%d\n",Left_Down_Find);
		}
		
		if(Right_Down_Find == 0
			&& abs(right_edge_line[i]   - right_edge_line[i+1]) >= 5
			&& abs(right_edge_line[i+1] - right_edge_line[i+2]) >= 5
			&& abs(right_edge_line[i+2] - right_edge_line[i+3]) >= 5
			&& (right_edge_line[i] - right_edge_line[i-2]) <= -8
			&& (right_edge_line[i] - right_edge_line[i-3]) <= -10
			&& (right_edge_line[i] - right_edge_line[i-4]) <= -10)
		{
			Right_Down_Find = i;
			//printf("find R_D :%d\n",Right_Down_Find);
		
		}
		if(Left_Down_Find != 0 && Right_Down_Find != 0)
		{
			break;
		}
	}
}
	
//---------------------------------------------------------------
//  找上面两个拐点，供十字使用
//  Find_Up_Point(start,end)
//---------------------------------------------------------------
void Find_Up_Point(int start,int end)
{
	int i,t;
	
//	Right_Up_Find = 0;
//	Left_Up_Find = 0;
	
	if(start < end)
	{
		//保证start>end
		t = start;
		start = end;
		end = t;
	}
	if(start >= START_ROW-5)
	{
		//下5行数据不稳定
		start = START_ROW-5;
	}
	if(end <= Search_Stop_Line)
	{
		end = Search_Stop_Line;
	}
	if(end <= 5)
	{
		end = 5;
	}
	
	for(i = start;i > end;i--)
	{
		if(Left_Up_Find == 0 
			&& abs(left_edge_line[i] - left_edge_line[i-1]) <= 5
			&& abs(left_edge_line[i-1] - left_edge_line[i-2]) <= 5
			&& abs(left_edge_line[i-2] - left_edge_line[i-3]) <= 5
			&& (left_edge_line[i] - left_edge_line[i+2]) >= 6
			&& (left_edge_line[i] - left_edge_line[i+3]) >= 8
			&& (left_edge_line[i] - left_edge_line[i+4]) >= 10)
		{
			
			Left_Up_Find = i;
			//printf("find L_U :%d\n",Left_Up_Find);
		}
		
		if(Right_Up_Find == 0
			&& abs(right_edge_line[i] - right_edge_line[i-1]) <= 5
			&& abs(right_edge_line[i-1] - right_edge_line[i-2]) <= 5
			&& abs(right_edge_line[i-2] - right_edge_line[i-3]) <= 5
			&& (right_edge_line[i] - right_edge_line[i+2]) <= -6
			&& (right_edge_line[i] - right_edge_line[i+3]) <= -8
			&& (right_edge_line[i] - right_edge_line[i+4]) <= -10)
		{
			Right_Up_Find = i;
			//printf("find R_U :%d\n",Right_Up_Find);
		}
		
		if(Left_Up_Find != 0 && Right_Up_Find != 0)
		{
			//printf("find both \n");
			break;
		}
	}
	
//	ips200_show_uint(0,120,Left_Up_Find,4);
//	ips200_show_uint(0,140,Right_Up_Find,4);
	
	if(abs(Right_Up_Find - Left_Up_Find) >= 30)
	{
		//纵向撕裂过大，视为误判
		Right_Up_Find = 0;
		Left_Up_Find = 0;
	}
}


//---------------------------------------------------------------
//  左边界延长
//  Lengthen_Left_Boundry()
//---------------------------------------------------------------
void Lengthen_Left_Boundry(int start,int end)
{
	int i,t;
	float k = 0;
	
	if(start >= START_ROW)
	{
		start = START_ROW;
	}
	if(start <= 0)
	{
		start = 0;
	}
	if(end >= START_ROW)
	{
		end = START_ROW;
	}
	if(end <= 0)
	{
		end = 0;
	}
	
	if(end < start)
	{
		//从上向下补线
		t = end;
		end = start;
		start = t;
	}
	
	if(start <= 5)
	{
		//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
		Left_Add_Line(left_edge_line[start],start,left_edge_line[end],end);
	}
	else
	{
		k = (float)(left_edge_line[start] - left_edge_line[start - 4])/5.0;
		for(i = start;i <= end;i++)
		{
			left_edge_line[i] = (int)(i - start)*k + left_edge_line[start];
			if(left_edge_line[i] >= START_ROW)
			{
				left_edge_line[i] = START_ROW;
			}
			else if(left_edge_line[i] <= 0)
			{
				left_edge_line[i] = 0;
			}
		}
	}
	
}

//---------------------------------------------------------------
//  右边界延长
//  Lengthen_Right_Boundry()
//---------------------------------------------------------------
void Lengthen_Right_Boundry(int start,int end)
{
	int i,t;
	float k = 0;
	
	if(start >= START_ROW)
	{
		start = START_ROW;
	}
	if(start <= 0)
	{
		start = 0;
	}
	if(end >= START_ROW)
	{
		end = START_ROW;
	}
	if(end <= 0)
	{
		end = 0;
	}
	
	if(end < start)
	{
		//从上向下补线
		t = end;
		end = start;
		start = t;
	}
	
	if(start <= 5)
	{
		Right_Add_Line(right_edge_line[start],start,right_edge_line[end],end);
	}
	else
	{
		k = (float)(right_edge_line[start] - right_edge_line[start - 4])/5.0;
		for(i = start;i < end;i++)
		{
			right_edge_line[i] = (int)(i - start)*k + right_edge_line[start];
		}
		if(right_edge_line[i] >= START_ROW)
		{
			right_edge_line[i] = START_ROW;
		}
		else if(right_edge_line[i] <= 0)
		{
			right_edge_line[i] = 0;
		}
	}
}





/**************************************************************************************************************************/
/****************************************************** 圆环 **************************************************************/
/**************************************************************************************************************************/
/*-------------------------------------------
	右环出环拉线点
-------------------------------------------*/
int Find_Right_Island_Out(int start_x,int start_y)
{
	int i,j;
	int Up_point_y = 0;
	
	if(start_x >= SEARCH_BORDER_R - 5)
	{
		start_x = SEARCH_BORDER_R - 5;
	}
	if(start_x <= SEARCH_BORDER_L + 5)
	{
		start_x = SEARCH_BORDER_L + 5;
	}
	
	if(start_y >= START_ROW-5)
	{
		start_y = START_ROW-5;
	}
	if(start_y >= MT9V03X_H - Search_Stop_Line)
	{
		start_y = MT9V03X_H - Search_Stop_Line;
	}
	if(start_y <= 5)
	{
		start_y = 5;
	}
	
	for(i = start_y;i >= Search_Stop_Line;i--)
	{
		if(bin_image[i][start_x] == 0 && bin_image[i-1][start_x] == 255 && bin_image[i-2][start_x] == 255)
		{
			Up_point_y = i;
		}
	}
		
	return Up_point_y;
}

//---------------------------------------------------------------
//  左下角点检测
//  Find_Left_Down_Points(START_ROW,20)
//----------------------------------------------------------------
int Find_Left_Down_Points(int start,int end)
{
	int i,t;
	int left_down_line = 0;
//	printf("Left_lose_Time = %d\n",Left_lose_Time);
	if(Left_lose_Time >= 0.9*MT9V03X_H)
	{
		//大部分丢线，没有判断拐点的意义
//		printf("here");
		return left_down_line;
	}
	if(start < end)
	{
		//保证start>end
		t = start;
		start = end;
		end = t;
	}
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end >= MT9V03X_H - Search_Stop_Line)
	{
		end = MT9V03X_H - Search_Stop_Line;
	}
	if(end <= 5)
	{
		end = 5;
	}
	
//	printf("start = %d\n",start);
//	printf("end = %d\n",end);
	
	for(i = start;i > end;i--)
	{
		if(left_down_line == 0 
			&& abs(left_edge_line[i] - left_edge_line[i+1]) <= 5
			&& abs(left_edge_line[i+1] - left_edge_line[i+2]) <= 5
			&& abs(left_edge_line[i+2] - left_edge_line[i+3]) <= 5
			&& (left_edge_line[i] - left_edge_line[i-2]) >= 8
			&& (left_edge_line[i] - left_edge_line[i-3]) >= 10
			&& (left_edge_line[i] - left_edge_line[i-4]) >= 12)
		{
//			printf("here");
			left_down_line = i;
			break;
		}
	}
	return left_down_line;
}

//---------------------------------------------------------------
//  左上角点检测
//  Find_Left_Up_Points(START_ROW,20)
//----------------------------------------------------------------
int Find_Left_Up_Points(int start,int end)
{
	int i,t;
	int left_up_line = 0;

	if(start < end)
	{
		t = start;
		start = end;
		end = t;
	}
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end >= MT9V03X_H-Search_Stop_Line)
	{
		end = MT9V03X_H-Search_Stop_Line;
	}
	if(end <= 5)
	{
		end = 5;
	}	
	
//	printf("start = %d\n",start);
//	printf("end = %d\n",end);
	
	for(i = start;i > end;i--)
	{
		if(left_up_line == 0 
			&& abs(left_edge_line[i] - left_edge_line[i-1]) <= 5
			&& abs(left_edge_line[i-1] - left_edge_line[i-2]) <= 5
			&& abs(left_edge_line[i-2] - left_edge_line[i-3]) <= 5
			&& (left_edge_line[i] - left_edge_line[i+2]) >= 6
			&& (left_edge_line[i] - left_edge_line[i+3]) >= 8
			&& (left_edge_line[i] - left_edge_line[i+4]) >= 10)
		{
//			printf("here IN\n");
			left_up_line = i;
			break;
		}
	}
	return left_up_line;
}

//---------------------------------------------------------------
//  右下角点检测
//  Find_Right_Down_Points(START_ROW,20)
//----------------------------------------------------------------
int Find_Right_Down_Points(int start,int end)
{
	int i,t;
	int right_down_line = 0;
	
	if(Right_lose_Time >= 0.9*MT9V03X_H)
	{
		//大部分丢线，没有判断拐点的意义
		return right_down_line;
	}
	if(start < end)
	{
		//保证start>end
		t = start;
		start = end;
		end = t;
	}
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end >= MT9V03X_H-Search_Stop_Line)
	{
		end = MT9V03X_H-Search_Stop_Line;
	}
	if(end <= 5)
	{
		end = 5;
	}
	
//	printf("start = %d\n",start);
//	printf("end = %d\n",end);
	
	for(i = start;i > end;i--)
	{
//		printf("deta%d = %d\n",i,right_edge_line[i] - right_edge_line[i-2]);
		if(right_down_line == 0
			&& abs(right_edge_line[i] - right_edge_line[i+1]) <= 5
			&& abs(right_edge_line[i+1] - right_edge_line[i+2]) <= 5
			&& abs(right_edge_line[i+2] - right_edge_line[i+3]) <= 5
			&& (right_edge_line[i] - right_edge_line[i-2]) <= -6
			&& (right_edge_line[i] - right_edge_line[i-3]) <= -8
			&& (right_edge_line[i] - right_edge_line[i-4]) <= -10)
		{
//			printf("herrrrrrrrrrrrrre\n");
			right_down_line = i;
			break;
		}
	}
	return right_down_line;
}

//---------------------------------------------------------------
//  右上角点检测
//  Find_Right_Up_Points(START_ROW,20)
//----------------------------------------------------------------
int Find_Right_Up_Points(int start,int end)
{
	int i,t;
	int right_up_line = 0;
	
	if(start < end)
	{
		//保证start>end
		t = start;
		start = end;
		end = t;
	}
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end >= MT9V03X_H-Search_Stop_Line)
	{
		end = MT9V03X_H-Search_Stop_Line;
	}
	if(end <= 5)
	{
		end = 5;
	}
	
	//printf("start = %d\n",start);
	//printf("end = %d\n",end);
	
	for(i = start;i > end;i--)
	{
		if(right_up_line == 0
			&& abs(right_edge_line[i] - right_edge_line[i-1]) <= 5
			&& abs(right_edge_line[i-1] - right_edge_line[i-2]) <= 5
			&& abs(right_edge_line[i-2] - right_edge_line[i-3]) <= 5
			&& (right_edge_line[i] - right_edge_line[i+2]) <= -5
			&& (right_edge_line[i] - right_edge_line[i+3]) <= -8
			&& (right_edge_line[i] - right_edge_line[i+4]) <= -10)
		{
			//printf("INNNN");
			right_up_line = i;
			break;
		}
	}
	return right_up_line;
}

//---------------------------------------------------------------
//  右赛道连续性检测
//  Continuity_Change_Right(start,end)
//----------------------------------------------------------------
int Continuity_Change_Right(int start,int end)
{
	int i,t;
	int continuity_change_flag = 0;
	if(Right_lose_Time >= 0.9*MT9V03X_H)
	{
		//大部分丢线没有判断的价值
		return 1;
	}
	
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end < 5)
	{
		end = 5;
	}
	if(start < end)
	{
		//从下向上检测连续性的
		t = start;
		start = end;
		end = t;
	}
	
	for(i = start;i > end;i--)
	{
		if(abs(right_edge_line[i] - right_edge_line[i-1]) > 7)
		{
			continuity_change_flag = i;
			break;
		}
	}
	
	return continuity_change_flag;
}

//---------------------------------------------------------------
//  左赛道连续性检测
//  Continuity_Change_Left(start,end)
//----------------------------------------------------------------
int Continuity_Change_Left(int start,int end)
{
	int i,t;
	int continuity_change_flag = 0;
	if(Left_lose_Time >= 0.9*MT9V03X_H)
	{
		return 1;
	}
	
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end < 5)
	{
		end = 5;
	}
	if(start < end)
	{
		//从下向上检测连续性的
		t = start;
		start = end;
		end = t;
	}
	
	for(i = start;i > end;i--)
	{
		if(abs(left_edge_line[i] - left_edge_line[i-1]) > 7)
		{
			continuity_change_flag = i;
			break;
		}
	}
	
	return continuity_change_flag;
}

//---------------------------------------------------------------
//  左赛道单调性突变检测
//  Monotonicity_Change_Left(start,end)
//----------------------------------------------------------------
int Monotonicity_Change_Left(int start,int end)
{
	int i;
	int monotonicity_change_line = 0;
	
	if(Left_lose_Time > 0.9*MT9V03X_H)
	{
		//丢线过多，不进行单调性判断
		return monotonicity_change_line;
	}
	
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end <= 5)
	{
		end = 5;
	}
	if(start <= end)
	{
		//因为是递减运算，入口反了就返回
		return monotonicity_change_line;
	}
	
	for(i = start;i >= end;i--)
	{
		if(left_edge_line[i] == left_edge_line[i+5] && left_edge_line[i] == left_edge_line[i-5]
			&& left_edge_line[i] == left_edge_line[i+4] && left_edge_line[i] == left_edge_line[i-4]
			&& left_edge_line[i] == left_edge_line[i+3] && left_edge_line[i] == left_edge_line[i-3]
			&& left_edge_line[i] == left_edge_line[i+2] && left_edge_line[i] == left_edge_line[i-2]
			&& left_edge_line[i] == left_edge_line[i+1] && left_edge_line[i] == left_edge_line[i-1])
		{
			//如果一堆单调性没有变化，不记录
			continue;
		}
		
		else if(left_edge_line[i] >= left_edge_line[i+5] && left_edge_line[i] >= left_edge_line[i-5]
				&& left_edge_line[i] >= left_edge_line[i+4] && left_edge_line[i] >= left_edge_line[i-4]
				&& left_edge_line[i] >= left_edge_line[i+3] && left_edge_line[i] >= left_edge_line[i-3]
				&& left_edge_line[i] >= left_edge_line[i+2] && left_edge_line[i] >= left_edge_line[i-2]
				&& left_edge_line[i] >= left_edge_line[i+1] && left_edge_line[i] >= left_edge_line[i-1])
		{
			//如果这个是最大的，取出
			monotonicity_change_line = i;
			break;
		}
	}
	return monotonicity_change_line;
}

//---------------------------------------------------------------
//  右赛道单调性突变检测
//  Monotonicity_Change_Right(start,end)
//----------------------------------------------------------------
int Monotonicity_Change_Right(int start,int end)
{
	int i;
	int monotonicity_change_line = 0;
	
	if(Right_lose_Time > 0.9*MT9V03X_H)
	{
		return monotonicity_change_line;
	}
	
	if(start >= START_ROW-5)
	{
		start = START_ROW-5;
	}
	if(end <= 5)
	{
		end = 5;
	}
	if(start <= end)
	{
		return monotonicity_change_line;
	}
	
	for(i = start;i >= end;i--)
	{
		if(right_edge_line[i] == right_edge_line[i+5] && right_edge_line[i] == right_edge_line[i-5]
			&& right_edge_line[i] == right_edge_line[i+4] && right_edge_line[i] == right_edge_line[i-4]
			&& right_edge_line[i] == right_edge_line[i+3] && right_edge_line[i] == right_edge_line[i-3]
			&& right_edge_line[i] == right_edge_line[i+2] && right_edge_line[i] == right_edge_line[i-2]
			&& right_edge_line[i] == right_edge_line[i+1] && right_edge_line[i] == right_edge_line[i-1])
		{
			continue;
		}
		
		else if(   right_edge_line[i] <= right_edge_line[i+5] && right_edge_line[i] <= right_edge_line[i-5]
				&& right_edge_line[i] <= right_edge_line[i+4] && right_edge_line[i] <= right_edge_line[i-4]
				&& right_edge_line[i] <= right_edge_line[i+3] && right_edge_line[i] <= right_edge_line[i-3]
				&& right_edge_line[i] <= right_edge_line[i+2] && right_edge_line[i] <= right_edge_line[i-2]
				&& right_edge_line[i] <= right_edge_line[i+1] && right_edge_line[i] <= right_edge_line[i-1])
		{
			monotonicity_change_line = i;
			break;
		}
	}
	return monotonicity_change_line;
}

//---------------------------------------------------------------
//  通过斜率定点补线 ---- 左
//  K_Add_Boundry_Left(start,end)
//----------------------------------------------------------------
void K_Add_Boundry_Left(float k,int startX,int startY,int endY)
{
	int i,t;
	
	if(startY >= START_ROW)
	{
		startY = START_ROW;
	}
	else if(startY <= 0)
	{
		startY = 0;
	}
	if(endY >= START_ROW)
	{
		endY = START_ROW;
	}
	else if(endY <= 0)
	{
		endY = 0;
	}
	
	if(startY <= endY)
	{
		t = startY;
		startY = endY;
		endY = t;
	}
	
	for(i = startY;i > endY;i--)
	{
		left_edge_line[i] = (int)((i - startY)/k + startX); //x = (y - y1)/k + x1
		if(left_edge_line[i] >= SEARCH_BORDER_R)
		{
			left_edge_line[i] = SEARCH_BORDER_R;
		}
		else if(left_edge_line[i] <= 0)
		{
			left_edge_line[i] = 0;
		}
	}
}

//---------------------------------------------------------------
//  通过斜率定点补线 ---- 右
//  K_Add_Boundry_Right(start,end)
//----------------------------------------------------------------
void K_Add_Boundry_Right(float k,int startX,int startY,int endY)
{
	int i,t;
	
	if(startY >= START_ROW)
	{
		startY = START_ROW;
	}
	else if(startY <= 0)
	{
		startY = 0;
	}
	if(endY >= START_ROW)
	{
		endY = START_ROW;
	}
	else if(endY <= 0)
	{
		endY = 0;
	}
	
	if(startY <= endY)
	{
		t = startY;
		startY = endY;
		endY = t;
	}
	
	for(i = startY;i > endY;i--)
	{
		right_edge_line[i] = (int)((i - startY)/k + startX);
		if(right_edge_line[i] >= SEARCH_BORDER_R)
		{
			right_edge_line[i] = SEARCH_BORDER_R;
		}
		else if(right_edge_line[i] <= 0)
		{
			right_edge_line[i] = 0;
		}
	}
}

//---------------------------------------------------------------
//  根据斜率画一条定直线
//  K_Draw_Line(k, 20,MT9V03X_H-1 ,0)
//----------------------------------------------------------------
void K_Draw_Line(float k, int startX, int startY,int endY)
{
	int endx = 0;
	
	///////////ips20_show_int(120,11*16,endY,3);
	if(startX >= SEARCH_BORDER_R)
	{
		startX = 179;
	}
	else if(startX <= SEARCH_BORDER_L)
	{
		startX = 0;
	}
	if(startY >= START_ROW)
	{
		startY = START_ROW;
	}
	else if(startY <= 0)
	{
		startY = 0;
	}
	if(endY >= START_ROW)
	{
		endY = START_ROW;
	}
	else if(endY <= 0)
	{
		endY = 0;
	}
	
//	startX = 179;
	endx = (int)((endY - startY)/k + startX);
	
//	ips200_show_int(120,7*16,startX,3);
//	ips200_show_int(120,8*16,startY,3);
//	ips200_show_int(120,9*16,endx,3);
//	ips200_show_int(120,10*16,endY,3);
//	printf("startX = %d\n",startX);
//	printf("startY = %d\n",startY);
//	printf("endx = %d\n",endx);
//	printf("endY = %d\n",endY);
	
	Draw_Line(startX,startY,endx,endY);
		
}

//---------------------------------------------------------------
//  画线 ---- 一条宽度为2的黑线
//  Draw_Line(k, 20,MT9V03X_H-1 ,0)
//----------------------------------------------------------------
void Draw_Line(int startX, int startY, int endX, int endY)
{
	int x,y,i;
	int start = 0,end = 0;
	
	if(startX >= SEARCH_BORDER_R)
	{
		startX = SEARCH_BORDER_R;
	}
	else if(startX <= SEARCH_BORDER_L)
	{
		startX = 0;
	}
	if(startY >= START_ROW)
	{
		startY = START_ROW;
	}
	else if(startY <= 0)
	{
		startY = 0;
	}
	if(endX >= SEARCH_BORDER_R)
	{
		endX = SEARCH_BORDER_R;
	}
	else if(endX <= SEARCH_BORDER_L)
	{
		endX = 0;
	}
	if(endY >= START_ROW)
	{
		endY = START_ROW;
	}
	else if(endY <= 0)
	{
		endY = 0;
	}
	
	if(startX == endX)
	{
		//如果是一条竖线
		if(startY > endY)
		{	//互换
			//从上到下
			start = endY;
			end = startY;
		}
		for(i = start;i < end;i++)
		{
			if(i <= 2)
			{
				//限幅
				i = 2;
			}
			bin_image[i][startX]   = 0;
			bin_image[i-1][startX] = 0;
			bin_image[i-2][startX] = 0;
		}
	}
	else if(startY == endY)
	{
		//如果是竖线
		if(startX > endX)
		{
			start = endX;
			end = startX;
		}
		for(i = start;i < end;i++)
		{
			if(startY <= 2)
			{
				startY = 2;
			}
			bin_image[startY][i]   = 0;
			bin_image[startY-1][i] = 0;
			bin_image[startY-2][i] = 0;
		}
	}
	else
	{
		//上边为 水平 竖直 的两种特殊情况，以下为正常情况
		
		//起始点矫正，让小的当开始
		if(startY > endY)
		{
			start = endY;
			end = startY;
		}
		else
		{
			start = startY;
			end = endY;
		}
		
		for(i = start;i < end;i++)
		{
			//纵向补线，保证每行都有黑点
			x = (int)(startX + (i - startY)*(endX - startX)/(endY - startY)); //x = (y -y1)/k + x1;
			if(x >= SEARCH_BORDER_R)
			{
				x = SEARCH_BORDER_R;
			}
			else if(x <= SEARCH_BORDER_L+2)
			{
				x = SEARCH_BORDER_L+2;
			}

			bin_image[i][x]   = 0;
			bin_image[i][x-1] = 0;
			bin_image[i][x-2] = 0;
			
			bin_image[i+1][x]   = 0;
			bin_image[i+1][x-1] = 0;
			bin_image[i+1][x-2] = 0;
		}
		
		
		if(startX > endX)
		{
			start = endX;
			end = startX;
		}
		else
		{
			start = startX;
			end = endX;
		}
		for(i = start;i < end;i++)
		{
			//横向补线，保证每列都有黑点
			y = (int)(startY + (i - startX)*(endY - startY)/(endX - startX)); //y = (x - x1)*k + y1;
			if(y >= START_ROW)
			{
				y = START_ROW;
			}
			else if(y <= 1)
			{
				y = 1;
			}
			
			bin_image[y][i] = 0;
			bin_image[y - 1][i] = 0;
		}
	}
	
}
//---------------------------------------------------------------
//	数字图像数组
//----------------------------------------------------------------
const uint8 Image_Flags[][9][8]=
{
	{                                //0
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //1
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //2
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //3
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0XFF,0XFF,0X00},
        {0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //4
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0X00,0XFF,0X00,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0XFF,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0XFF,0X00,0X00,0X00},
	},
	{                                        //5
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //6
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //7
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0XFF,0X00,0X00,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //8
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
	{                                        //9
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0XFF,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0XFF,0X00},
        {0X00,0X00,0X00,0X00,0X00,0XFF,0X00,0X00},
        {0X00,0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X00},
        {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00},
	},
};
/*-------------------------------------------------------------------------------------------------------------------
  @brief     元素标志位显示
  @param     二值化图片数组
  @return    null
  Sample     Image_Flag_Show(MT9V03X_W,image_two_value,Island_State);
  @note      要在图片显示前使用，传入标志位可更改
-------------------------------------------------------------------------------------------------------------------*/
void Image_Flag_Show(uint8 MT9V03XW,uint8(*InImg)[MT9V03XW],uint8 image_flag)
{
    for(uint8 H=1;H<8;H++)
    {
        for(uint8 W=1;W<7;W++)
        {
            switch (image_flag)
            {
                case 0:     *(*(InImg+H)+W)=Image_Flags[0][H][W];    break;
                case 1:     *(*(InImg+H)+W)=Image_Flags[1][H][W];    break;
                case 2:     *(*(InImg+H)+W)=Image_Flags[2][H][W];    break;
                case 3:     *(*(InImg+H)+W)=Image_Flags[3][H][W];    break;
                case 4:     *(*(InImg+H)+W)=Image_Flags[4][H][W];    break;
                case 5:     *(*(InImg+H)+W)=Image_Flags[5][H][W];    break;
                case 6:     *(*(InImg+H)+W)=Image_Flags[6][H][W];    break;
                case 7:     *(*(InImg+H)+W)=Image_Flags[7][H][W];    break;
                case 8:     *(*(InImg+H)+W)=Image_Flags[8][H][W];    break;
                case 9:     *(*(InImg+H)+W)=Image_Flags[9][H][W];    break;
                default:    break;
            }
        }
    }
}

