#include "Search_line.h"

/************************************ 二值化图像 ************************************/
	/***** 转存图像 *****/
uint8 original_image[MT9V03X_H][MT9V03X_W] = {0};
	/***** 二值化图像 *****/
uint8 bin_image[MT9V03X_H][MT9V03X_W] = {0};
uint8 image_thereshold = 0;

/************************************ 搜线 ************************************/
	/***** 最长白列 *****/
uint8 reference_col = 0;	//动态参考列
uint8 remote_distance[SEARCH_IMAGE_W]    = {0};  //白点远端距离
uint8 Search_Stop_Line = 0;	//截止行
	/***** 搜线 *****/
uint8 left_edge_line[SEARCH_IMAGE_H]  = {0};//左边界
uint8 right_edge_line[SEARCH_IMAGE_H] = {0};//右边界
uint8 middle_line[SEARCH_IMAGE_H] = {0};	//中线

uint8 Boundry_Start_Left  = 0; //第一个非丢线点
uint8 Boundry_Start_Right = 0;

uint8 Left_lose_flag[SEARCH_IMAGE_H]  = {0};
uint8 Right_lose_flag[SEARCH_IMAGE_H] = {0};

uint8 Left_lose_Time  = 0;
uint8 Right_lose_Time = 0;
uint8 Both_lose_time  = 0;

uint8 track_width[SEARCH_IMAGE_H] = {0};

/************************************ 十字检测 ************************************/
	/***** 补线搜索 *****/
uint8 Right_Down_Find = 0;
uint8 Left_Down_Find = 0;
uint8 Right_Up_Find = 0;
uint8 Left_Up_Find  = 0;

	/***** 各个元素标志位 *****/
uint8 Cross_Flag = 0;
uint8 Island_State = 0;
uint8 Zebra_Flag = 0;
uint8 Stop_Flag  = 0;
uint8 Start_Count = 0;      // 起跑线通过计数器
uint8 Last_Zebra_State = 0; // 上一次斑马线状态（用于检测下降沿）
uint8 Stop_State = 0;	//停车状态机


/**************************************************************************************************************************/
/**************************************************** 二值化图像 **********************************************************/
/**************************************************************************************************************************/
//---------------------------------------------------------------
//  大津法二值化
//	otsuThreshold()
//	返回阈值
//---------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
#define GrayScale 256
	static uint8 thresholdHistory[5] = {0};  // 历史阈值存储
    static uint8 historyIndex 	= 0;	//前几次的角标
    static uint8 initialized 	= 0;	//初始化
    static uint32 prevIntegral 	= 0;	//上次积分结果
	
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};
	
	uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;
	
	
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }

    for (MinValue = 0; MinValue <= 255 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
	return Threshold;
}

//---------------------------------------------------------------
//  转存图像
//	get_image(mt9v03x_image)
//	得到转存图像 original_image[]
//---------------------------------------------------------------
void get_image(uint8(*image)[MT9V03X_W])
{
#define compress_num 1
	uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < MT9V03X_H; i += compress_num)          
    {
        for (j = 0; j <MT9V03X_W; j += compress_num)     
        {
            original_image[row][line] = image[i][j];
			line++;
        }
        line = 0;
        row++;
	}
}

//---------------------------------------------------------------
//  得到二值化图像
//	turn_to_bin()
//	得到二值化图像 bin_image[]
//---------------------------------------------------------------
void turn_to_bin(void)
{
	uint8 i,j;
	image_thereshold = otsuThreshold(original_image[0], MT9V03X_W, MT9V03X_H);
	uint8 final_threshold = 0;
	for(i = 0;i<MT9V03X_H;i++)
	{
		for(j = 0;j<MT9V03X_W;j++)
		{
			if(original_image[i][j]>image_thereshold)
			{
				bin_image[i][j] = 255;
			}	
			else 
			{
				bin_image[i][j] = 0;
			}
		}
	}
}

/**************************************************************************************************************************/
/******************************************************* 搜线 *************************************************************/
/**************************************************************************************************************************/
//---------------------------------------------------------------
//  搜索最长白列
//	search_reference_col(bin_image)
//---------------------------------------------------------------
void search_reference_col(const uint8 *image)
{
	int col,row;
	int16 temp1 = 0,temp2 = 0,temp3 = 0;
	
	for(col = 0;col < SEARCH_IMAGE_W;col++)
	{
		remote_distance[col] = SEARCH_IMAGE_H - 1;  //参考列还原到最近段
	}
	
	for(col = 0;col < SEARCH_IMAGE_W;col += CONTRASTOFFSET)
	{
		for(row = START_ROW;row > STOP_ROW;row -= CONTRASTOFFSET)
		{
			//最下边一行--temp1
			temp1 = *(image + row * SEARCH_IMAGE_W +col);              //获取当前点灰度值
			temp2 = *(image + (row - CONTRASTOFFSET) * SEARCH_IMAGE_W +col); //获取对比点灰度值
			temp3 = *(image + (row - 2*CONTRASTOFFSET) * SEARCH_IMAGE_W +col);
			
			if(temp2 == 255)            					//如果是白点，直接跳过
			{
				continue;
			}
			if(temp1 == 0)            					    //如果是黑点，则不进行对比直接赋值
			{
				remote_distance[col] = (uint8)row;
				break;
			}
			//防止搜飞
			if(row == STOP_ROW)
			{
				remote_distance[col] = (uint8)row;
				break;
			}
		}
	}
	
	//reference_col --> x
	reference_col = find_extreme_value(remote_distance,10,SEARCH_IMAGE_W - 10,0) + CONTRASTOFFSET;
	Search_Stop_Line = remote_distance[reference_col-1];
	
//	for(int b = SEARCH_BORDER_L;b <= SEARCH_BORDER_R;b++)
//	{
//		printf("remote_distance[%d] = %d\n",b,remote_distance[b]);
//		
//	}
	//最长白列不会出现在图的左右边界处
	reference_col = func_limit_ab(reference_col,25,SEARCH_IMAGE_W - 25);  //防止斜入十字时错误
	
	
		
}

//---------------------------------------------------------------
//  新的搜线
//  New_Search_Line()
//---------------------------------------------------------------
void New_Search_Line(const uint8 (*image)[MT9V03X_W])
{
	Left_lose_Time  = 0;
	Right_lose_Time = 0;
	Both_lose_time  = 0;
	
	int i,j;
	
	memset(left_edge_line ,0,sizeof(left_edge_line));
	memset(right_edge_line,MT9V03X_W,sizeof(right_edge_line));
	
	for(i=START_ROW;i>=STOP_ROW;i--)//从最底下往上扫描
    {
        for(j=reference_col;j<SEARCH_BORDER_R;j++)//往右扫描
        {
            if(image[i][j]==0xff&&image[i][j+1]==0x00&&image[i][j+2]==0x00)//白黑黑，找到右边界
            {
                right_edge_line[i]=j;
                break;//跳出，找到本行边界就没必要循环下去了
            }
            else
            {
                right_edge_line[i]=j;//没找到右边界，把屏幕最右赋值给右边界

            }
        }
        for(j=reference_col;j>SEARCH_BORDER_L;j--)//往左边扫描
        {
            if(image[i][j]==0xff&&image[i][j-1]==0x00&&image[i][j-2]==0x00)//黑黑白认为到达左边界
            {
                left_edge_line[i]=j;
                break;//跳出，找到本行边界就没必要循环下去了
            }
            else
            {
                left_edge_line[i]=j;//找到头都没找到边，就把屏幕最左右当做边界
            }
        }
	}
	
	/**** 边线数据处理 ****/
	Right_lose_Time = 0;
	Left_lose_Time  = 0;
	Both_lose_time  = 0;
	Boundry_Start_Left  = 0;
	Boundry_Start_Right = 0;
	
	for(int i = START_ROW;i >= 1;i--)
	{
		if(left_edge_line[i] != SEARCH_BORDER_L+1)
		{
			Left_lose_flag[i] = 0;
		}
		else
		{
			Left_lose_flag[i] = 1;
		}
		
		if(right_edge_line[i] != SEARCH_BORDER_R-1)
		{
			Right_lose_flag[i] = 0;
			
		}
		else
		{
			Right_lose_flag[i] = 1;
//			printf("pass:%d\n",i);
		}
	}
	
//	for(int i = START_ROW;i >= 1;i--)
//	{
//		printf("saikuan[%d]=%d\n",i,right_edge_line[i] - left_edge_line[i]);
//	}
//	printf("trace:\n");
	for(int i = START_ROW;i >= 1;i--)
	{
		if(Right_lose_flag[i]==1)
		{
			//右丢线
			Right_lose_Time++;
			
		}
		if(Left_lose_flag[i]==1)
		{
			//左丢线
			Left_lose_Time++;
		}
		if(Right_lose_flag[i]==1 && Left_lose_flag[i]==1)
		{
			//双边丢线
			Both_lose_time++;
		}
		if(Boundry_Start_Left == 0 && Left_lose_flag[i] != 1)
		{
			//记录第一个非丢线位置
			Boundry_Start_Left = i;
		}
		if(Boundry_Start_Right == 0 && Right_lose_flag[i] != 1)
		{
			//记录第一个非丢线位置
			Boundry_Start_Right = i;
		}
		track_width[i] = right_edge_line[i] - left_edge_line[i]; //赛宽
	}
}

//---------------------------------------------------------------
//  显示边界
//  Show_Boundry()
//---------------------------------------------------------------
void Show_Boundry()
{
	int16 i;
	for(i = START_ROW;i > Search_Stop_Line;i--)
	{
		bin_image[i][left_edge_line[i]+1] = 0;
		bin_image[i][(left_edge_line[i]+right_edge_line[i])>>1] = 0;
		bin_image[i][right_edge_line[i]-1] = 0;
		
		//bin_image[i][reference_col] = 0;
	}
	Image_Flag_Show(MT9V03X_W,bin_image,Island_State);
}

/**************************************************************************************************************************/
/**************************************************** 十字检测 ************************************************************/
/**************************************************************************************************************************/
//---------------------------------------------------------------
//  十字检测
//  Cross_Detect()
//---------------------------------------------------------------
void Cross_Detect()
{
	int down_search_start = 0; //下角点搜索开始行
	Cross_Flag = 0;
	
	if(Control_Mode != 0)
	{
		return;
	}
	
	if(Island_State == 0) //与圆环还有找箱子岔开
	{
		Right_Up_Find = 0;
		Left_Up_Find = 0;
		
		//	ips200_show_uint(0,160,Both_lose_time,4);
		if(Both_lose_time >= 7)
		{
			//printf("Im in\n");
			//有双边丢线再开始找角点
			Find_Up_Point(START_ROW,STOP_ROW);
			if(Left_Up_Find == 0 || Right_Up_Find == 0)
			{
				//只要没有同时找到两个就不进行下一步
				
				return;
			}
		}
		
		if(Left_Up_Find != 0 && Right_Up_Find != 0)
		{
			Cross_Flag = 1;
			down_search_start = Left_Up_Find>Right_Up_Find?Left_Up_Find:Right_Up_Find; //两上拐点坐标靠下的作为下拐点搜索上限
			Find_Down_Point(START_ROW - 5,down_search_start + 2);
			
			//下点不可能比上点还靠上
			if(Left_Down_Find <= Left_Up_Find)
			{
				Left_Down_Find = 0;
			}
			if(Right_Down_Find <= Right_Up_Find)
			{
				Right_Down_Find = 0;
			}
			
			if(Left_Down_Find != 0 && Right_Down_Find != 0)
			{
				//四个点都在，很少有
				Left_Add_Line(left_edge_line[Left_Up_Find],Left_Up_Find,left_edge_line[Left_Down_Find],Left_Down_Find);
				Right_Add_Line(right_edge_line[Right_Up_Find],Right_Up_Find,right_edge_line[Right_Down_Find],Right_Down_Find);
			}
			else if(Left_Down_Find == 0 && Right_Down_Find != 0)
			{
				//三个点，只有右下
				Lengthen_Left_Boundry(Left_Up_Find-1,START_ROW);
				Right_Add_Line(right_edge_line[Right_Up_Find],Right_Up_Find,right_edge_line[Right_Down_Find],Right_Down_Find);
			}
			else if(Left_Down_Find != 0 && Right_Down_Find == 0)
			{
				//三个点，只有左下
				Left_Add_Line(left_edge_line[Left_Up_Find],Left_Up_Find,left_edge_line[Left_Down_Find],Left_Down_Find);
				Lengthen_Right_Boundry(Right_Up_Find-1,START_ROW);
			}
			else if(Left_Down_Find == 0 && Right_Down_Find == 0)
			{
				//两个点，下拐点全丢
				Lengthen_Left_Boundry(Left_Up_Find-1,START_ROW);
				Lengthen_Right_Boundry(Right_Up_Find-1,START_ROW);
			}
			
		}
		else
		{
			//清除标志位
			Cross_Flag = 0;
		}
	}
//	ips200_show_uint(0,100,Cross_Flag,4);
//	ips200_show_uint(0,120,Left_Up_Find,4);
//	ips200_show_uint(0,140,Right_Up_Find,4);
//	ips200_show_uint(0,160,Left_Down_Find,4);
//	ips200_show_uint(0,180,Right_Down_Find,4);
}

/**************************************************************************************************************************/
/******************************************************* 误差 *************************************************************/
/**************************************************************************************************************************/
const uint8 Standard_Road_Wide[MT9V03X_H]=//标准赛宽
{	 23, 23, 25, 26, 27, 29, 30, 31, 32, 34,
	 35, 36, 38, 39, 40, 42, 42, 44, 46, 46,
	 48, 49, 50, 52, 52, 54, 56, 57, 58, 59,
	 60, 62, 63, 64, 65, 67, 68, 69, 70, 71,
	 73, 74, 75, 77, 78, 79, 81, 81, 83, 85,
	 85, 87, 89, 89, 91, 92, 93, 94, 96, 97,
	 98, 99,101,102,103,104,106,107,108,110,
	 110,112,113,114,116,116,118,118,120,120
};
const uint8 Weight[MT9V03X_H]=
{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 2, 2, 4, 4,            
        6, 8, 8, 8,10,12,12,12,10, 8,              
        8, 6, 6, 4, 4, 2, 2, 2, 1, 1,            
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,              
};
const uint8 Weight_2[MT9V03X_H]=
{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,            
        1, 4, 4, 1, 1, 1, 1, 1, 2, 2,              
        4, 4, 6, 6, 8, 8, 14, 18, 20, 22,            
        26, 24, 22, 20, 20, 18, 18, 16, 16, 16,              
        16, 16, 16, 5, 4, 4, 4, 1, 1, 1,              
};

const uint8 Weight_3[MT9V03X_H]=
{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,            
        1, 1, 1, 1, 1, 1, 1, 1, 2, 2,              
        4, 4, 6, 6, 8, 8, 14, 16, 16, 18,            
        20, 20, 18, 18, 18, 18, 12, 12, 10, 8,              
        8, 8, 8, 1, 1, 1, 1, 1, 1, 1,              
};
const uint8 Weight_4[MT9V03X_H]=
{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 3,            
        3, 3, 3, 3, 3, 1, 1, 1, 2, 2,    
		2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
        4, 4, 6, 6, 8, 10, 14, 14, 16, 18,            
        20, 20, 24, 26, 24, 20, 18, 12, 10, 8,              
                      
};
const uint8 Weight_5[MT9V03X_H]=
{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,             
        1, 1, 1, 1, 1, 1, 1, 1, 1, 3,            
        3, 3, 3, 3, 3, 1, 1, 1, 2, 2,    
		2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
        4, 4, 6, 6, 8, 12, 14, 14, 18, 22,            
        24, 26, 24, 22, 22, 22, 18, 10, 1, 1,              
                      
};

/*-------------------------------------------
	误差计算
	Err_Sum() 			---> 权重赋值
	Err_Sum_INisland() 	---> 选定行控制
-------------------------------------------*/
float Err_Sum()
{
	int i;
	float err = 0;
	float weight_count = 0;
	
	for(i = START_ROW;i > Search_Stop_Line;i--)
	{
		err += (MT9V03X_W/2 - ((right_edge_line[i] + left_edge_line[i])>>1))*Weight_4[i];
		weight_count += Weight_4[i];
	}
	err = err/weight_count;
	
	return err;
}

float Err_Sum_INisland_L()
{
	int i;
	float err = 0;
	float weight_count = 0;
	
	for(i = START_ROW;i > Search_Stop_Line;i--)
	{
		err += (MT9V03X_W/2 - ((left_edge_line[i] + Standard_Road_Wide[i]/2)>>1))*Weight_4[i];
		weight_count += Weight_4[i];
	}
	err = err/weight_count;
	
	return err;
}
float Err_Sum_INisland_R()
{
	int i;
	float err = 0;
	float weight_count = 0;
	
	for(i = START_ROW;i > Search_Stop_Line;i--)
	{
		err += (MT9V03X_W/2 - ((right_edge_line[i] - Standard_Road_Wide[i]/2)>>1))*Weight_4[i];
		weight_count += Weight_4[i];
	}
	err = err/weight_count;
	
	return err;
}

/**************************************************************************************************************************/
/*************************************************** 斑马线检测 ***********************************************************/
/**************************************************************************************************************************/
/*-----------------------------------------
	斑马线检测停车
	最长白列撕裂
-----------------------------------------*/
void Zebra_Stop()
{
	int i = 0,j = 0;
	uint32 break_num = 0;
	

	//防止误判
	if(Cross_Flag != 0
		|| Island_State != 0
		|| Cross_Flag != 0
		|| Stop_Flag != 0
	)
	{
		return;
	}



//	ips200_show_int(60, 9 * 16, break_num, 3);
//	ips200_show_int(60, 10 * 16, Stop_State, 3);
//	ips200_show_int(60, 11 * 16, Stop_Flag, 3);
	
	/*** 状态0 ***/
	/*-----------------------------------------
	Stop_State = 0 : 在斑马线前发车时,看到斑马线
	-----------------------------------------*/
	if(Stop_State == 0)
	{
		if(Search_Stop_Line <= WHITE_STOP_LINE)//视野很长
		{
			for(j=WHITE_RANGE;j<=MT9V03X_W-WHITE_RANGE;j++)
			{
				if(abs(remote_distance[j]-remote_distance[j+1])>=WHITE_POINTS_BREAK)
				{

					break_num++;
				}
			}
		}
		if(break_num >= 12)
		{
			Stop_State = 1;
			Zebra_Flag = 1;
		}
		else
		{
			Stop_State = 0;
			Zebra_Flag = 0;
		}
	}
	/*** 状态1 ***/
	/*-----------------------------------------
	Stop_State = 1 : 发车后,第一次经过斑马线
	-----------------------------------------*/
	else if(Stop_State == 1
			&& Zebra_Flag == 1
			)
	{
		if(Search_Stop_Line <= WHITE_STOP_LINE)//视野很长
		{
			for(j=WHITE_RANGE;j<=MT9V03X_W-WHITE_RANGE;j++)
			{
				if(abs(remote_distance[j]-remote_distance[j+1])>=WHITE_POINTS_BREAK)
				{
					break_num++;
				}
			}
		}
		if(break_num >= 8)
		{
			return;
		}
		if(break_num <= 4)
		{
			Stop_State = 2;
			Zebra_Flag = 0;
		}
	}
	/*** 状态2 ***/
	/*-----------------------------------------
	Stop_State = 2 : 寻找停车的斑马线
	-----------------------------------------*/
	else if(Stop_State == 2
			&& Zebra_Flag == 0)
	{
		if(Search_Stop_Line <= WHITE_STOP_LINE)//视野很长
		{
			for(j=WHITE_RANGE;j<=MT9V03X_W-WHITE_RANGE;j++)
			{
				if(abs(remote_distance[j]-remote_distance[j+1])>=WHITE_POINTS_BREAK)
				{
					break_num++;
				}
			}
		}
		if(break_num >= 12)
		{
			Stop_State = 3;
			Zebra_Flag = 1;
		}
	}
	/*** 状态3 ***/
	/*-----------------------------------------
	Stop_State = 3 : 第二次经过斑马线,停车
	-----------------------------------------*/
	else if(Stop_State == 3)
	{
		if(Search_Stop_Line <= WHITE_STOP_LINE)//视野很长
		{
			for(j=WHITE_RANGE;j<=MT9V03X_W-WHITE_RANGE;j++)
			{
				if(abs(remote_distance[j]-remote_distance[j+1])>=WHITE_POINTS_BREAK)
				{
					break_num++;
				}
			}
		}
		if(break_num >= 12)
		{
			return;
		}
		if(break_num <= 4)
		{
			Stop_State = 4;
		}
		
	}
	/*** 状态4 ***/
	/*-----------------------------------------
	Stop_State = 4 : 停止运动
	-----------------------------------------*/
	else if(Stop_State == 4
			&& Zebra_Flag == 1)
	{
		Stop_Flag = 1;
	}
}

