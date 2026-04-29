#ifndef __BINARYZATION_H__
#define __BINARYZATION_H__
//#include "headfile.h"
#include "zf_common_headfile.h"
extern uint8  mt9v03x_image_Bina[MT9V03X_H][MT9V03X_W]; //二值化后的图像
 
uint8 otsu_Threshold_fast(uint8*image);                 //快速大津法二值化
void Set_image_towvalues(uint8 value);                  //将图像二值化
//void get_my_image(uint8 *image);                        //转存图像

#endif 