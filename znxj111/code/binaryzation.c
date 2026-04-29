#include "zf_common_headfile.h"
#include "binaryzation.h"

uint8 mt9v03x_image_Bina[MT9V03X_H][MT9V03X_W];

#define grayscale 256
//---------------------------------------------------------------
//  快速大津法  
//  pixcel_sum = width*height/4
//  otsu_Threshold_fast(mt9v03x_image[0])  传入原图像
//---------------------------------------------------------------
uint8 otsu_Threshold_fast(uint8*image)
{
	uint8 threshold = 0;
	uint8 last_threshold = 0;
	uint16 width = MT9V03X_W;
	uint16 height = MT9V03X_H;
	int pixcel_max = 0;
	int pixcel_min = 255;
	int pixcel_count[grayscale] = {0}; //各像素grayscale个数
	float pixcel_pro[grayscale] = {0}; //各像素grayscale所占百分比
	int i,j,
		pixcel_sum = width*height/4;
	uint8* data = image;
	
	//清空数组
//	for(i = 0;i < grayscale;i++)
//	{
//		pixcel_count[i] = 0;
//		pixcel_pro[i] = 0;
//	}
	uint32 gray_sum = 0;
	
	//统计灰度图中每个像素值所占的个数
	for(i = 0;i < height;i += 2)
	{
		for(j = 0;j < width;j += 2)
		{
			//二维数组a[0][1] == a[0]+1
			pixcel_count[(int)data[i * width + j]]++; //将当前的点的像素值作为计数数组的下标
			gray_sum += (int)data[i * width + j];
			if(data[i * width + j] > pixcel_max){pixcel_max = data[i * width + j];}
			if(data[i * width + j] < pixcel_min){pixcel_min = data[i * width + j];}
		}
	}
	
	//计算每个像素值的点在整幅图像中的占比
	for(i = pixcel_min;i < pixcel_max;i++)
	{
		pixcel_pro[i] = (float)pixcel_count[i]/pixcel_sum;
		
	}
	
	//遍历灰度级
	float w0,w1,u0_tmp,u1_tmp,u0,u1,u,deltaTmp,deltaMax;
	
	w0 = w1 = u0_tmp = u1_tmp = u0= u1 = u = deltaTmp = deltaMax = 0;
	for(j = pixcel_min;j < pixcel_max;j++)
	{
		w0 += pixcel_pro[j]; 		 //背景部分比例
		u0_tmp += j * pixcel_pro[j]; //背景部分 灰度值*每个点灰度值比例
		
		w1 = 1 - w0;       //前景部分比例
		u1_tmp = gray_sum/pixcel_sum - u0_tmp; //前景部分 灰度值/图像面积-背景部分 得到前景灰度值
		
		u0 = u0_tmp / w0;    //背景平均灰度
		u1 = u1_tmp / w1;    //前景平均灰度
		u = u0_tmp + u1_tmp; //全局平均灰度
		deltaTmp = (float)(w0 * w1 * (u0 - u1) * (u0 - u1));
		if(deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = (uint8)j;
		}
		if(deltaTmp < deltaMax)
		{
			break;
		}		
	}	
	return threshold;
}

//---------------------------------------------------------------
//  二值化
//  value是大津法得到的threshold
//  Set_image_towvalues(Bina_threshold)
//---------------------------------------------------------------
void Set_image_towvalues(uint8 value)
{
    uint8 temp_valude;//暂存灰度值
    for(uint8 i = 0;i < MT9V03X_H;i++)   //高
    {
      for(uint8 j = 0;j < MT9V03X_W;j++) //宽
      {
          temp_valude = mt9v03x_image[i][j];
          if(temp_valude < value)
          {
              mt9v03x_image_Bina[i][j] = 0;  //黑
          }
          else
          {
              mt9v03x_image_Bina[i][j] = 255;//白
          }
      }
 
    }
}

////---------------------------------------------------------------
////  图像转存压缩
////  get_my_image(mt9v03x_image[0])
////---------------------------------------------------------------
//uint8 my_original_image[MT9V03X_H][MT9V03X_W]; //转存，防止直接调用图像导致bug
//uint8 otsu_bina_threashold;

//void get_my_image(uint8 *image)
//{
//#define compress_num 1          //压缩图像的话，更改倍数
//	uint8 i,j,row = 0,line = 0;
//	for(i = 0; i < MT9V03X_H; i += compress_num)
//	{
//		for (j = 0; j <MT9V03X_W; j += compress_num)     
//        {
//            my_original_image[row][line] = mt9v03x_image[i][j];
//			line++;
//        }
//        line = 0;
//        row++;
//	}
//}



