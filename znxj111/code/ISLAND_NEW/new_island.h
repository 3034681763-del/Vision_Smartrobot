#ifndef _NEW_ISLAND_H_
#define _NEW_ISLAND_H_
#include "zf_common_headfile.h"

typedef struct
{
	int T_M1;
	int T_M2;
	int T_M3;
	int T_M4;
	int T_M5;
	int T_M6;
	int	Turn_Angle;
}Island_Angle;

extern int Left_Island_Flag_new ;
extern int Right_Island_Flag_new;

void Island_Detect_new();





#endif
