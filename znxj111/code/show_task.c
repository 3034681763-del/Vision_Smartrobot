#include "zf_common_headfile.h"

//#include "zf_common_headfile.h"
//#include "motor.h"
//#include "math.h"
//uint8 Openart_uart_rx_buff[2]={1,1};
//uint8 Openart_uart_rx_buff[15] = {
//    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
//};
//uint8 object_new[4][16] ;
//uint8 object_new_1[8][16] ;
// uint8 mouse[8][16];
 uint8_t mouse_s[8][16];
 uint8_t keyboard_s[8][16];
 uint8_t Monitor_s[8][16]; 
 uint8_t Headphones_s[8][16];
 uint8_t Speakers_s[8][16];
 uint8_t Printe_s[8][16];
 uint8_t Mobile_phone_s[8][16];
 uint8_t Wrench_s[8][16];
 uint8_t Screwdriver_s[8][16];
 uint8_t Electric_drill_s[8][16];
 uint8_t Pliers_s[8][16];
 uint8_t Multimeter_s[8][16];
 uint8_t Oscilloscope_s[8][16];
 uint8_t Soldering_iron_s[8][16];
 uint8_t Tape_measure_s[8][16];
 
uint8 Control_Mode_new_0  =2;   
uint8 Control_Mode_new_1 =2 ;   
uint8 Control_Mode_new_2 =2 ;   
uint8 Control_Mode_new_3  =2;   
uint8 Control_Mode_new_4 =2 ;   
uint8 Control_Mode_new_5 =2 ;   
uint8 Control_Mode_new_6 =2 ;  
uint8 Control_Mode_new_7 =2;   
uint8 Control_Mode_new_8 =2 ;   
uint8 Control_Mode_new_9 =2;
uint8 Control_Mode_new_10 =2;
uint8 Control_Mode_new_11 =2;
uint8 Control_Mode_new_12 =2;
uint8 Control_Mode_new_13 =2;
uint8 Control_Mode_new_14 =2;
uint8 Control_Mode_new_15 =2;
uint8 Control_Mode_new_16 =2;
uint8 Control_Mode_new_17 =2;
uint8 Control_Mode_new_18 =2;
uint8 Control_Mode_new_19 =2;
uint8 Control_Mode_new_20 =2;
uint8 Control_Mode_new_21 =2;
uint16 Correct_Mode_show=0;

uint8_t target_font[20][16];  // 用于存储最终选择的点阵数据
uint16 True_digital_0=0;
uint16 True_digital_1=0;
uint16 True_digital_2=0;
uint16 True_digital_3=0;
uint16 True_digital_4=0;
uint16 True_digital_5=0;
uint16 True_digital_6=0;
uint16 True_digital_7=0;
uint16 True_digital_8=0;
uint16 True_digital_9=0;
uint16 True_digital_10=0;
uint16 True_digital_11=0;
uint16 True_digital_12=0;
uint16 True_digital_13=0;
uint16 True_digital_14=0;
uint16 True_digital_15=0;
uint16 True_digital_16=0;
uint16 True_digital_17=0;
uint16 True_digital_18=0;
uint16 True_digital_19=0;
uint16 True_digital_20=0;
uint16 True_digital_21=0;
//uint16 True_digital_12=0;
//uint16 True_digital_13=0;
//uint16 True_digital_14=0;


//uint8_t True_digital = 0;    // 用于存储数字值
uint8_t object_show_0[20][16];
uint8_t object_show_1[20][16];
uint8_t object_show_2[20][16];
uint8_t object_show_3[20][16];
uint8_t object_show_4[20][16];
uint8_t object_show_5[20][16];
uint8_t object_show_6[20][16];
uint8_t object_show_7[20][16];
uint8_t object_show_8[20][16];
uint8_t object_show_9[20][16];
uint8_t object_show_10[20][16];
uint8_t object_show_11[20][16];
uint8_t object_show_12[20][16];
uint8_t object_show_13[20][16];
uint8_t object_show_14[20][16];
uint8_t object_show_15[20][16];
uint8_t object_show_16[20][16];
uint8_t object_show_17[20][16];
uint8_t object_show_18[20][16];
uint8_t object_show_19[20][16];
uint8_t object_show_20[20][16];
uint8_t object_show_21[20][16];
//uint8_t object_show_14[20][16];

void OpenArt_Data_judje()
{
	if(Openart_uart_rx_buff[0] ==1)
	{
			memcpy(target_font, Wrench, sizeof(Wrench));
	}
	else if(Openart_uart_rx_buff[0] ==2)
	{
		memcpy(target_font, Soldering_iron, sizeof(Soldering_iron));
	}
	else if(Openart_uart_rx_buff[0] ==3)
	{
		memcpy(target_font, Electric_drill, sizeof(Electric_drill));
	}
		else if(Openart_uart_rx_buff[0] ==4)
	{
		memcpy(target_font, Tape_measure, sizeof(Tape_measure_s));
	}
		else if(Openart_uart_rx_buff[0] ==5)
	{
		memcpy(target_font, Screwdriver, sizeof(Screwdriver));
	}
		else if(Openart_uart_rx_buff[0] ==6)
	{
		memcpy(target_font, Pliers, sizeof(Pliers));
	}
		else if(Openart_uart_rx_buff[0] ==7)
	{
		memcpy(target_font, Oscilloscope, sizeof(Oscilloscope));
	}
		else if(Openart_uart_rx_buff[0] ==8)
	{
		memcpy(target_font, Multimeter, sizeof(Multimeter));
	}
		else if(Openart_uart_rx_buff[0] ==9)
	{
		memcpy(target_font, Printe, sizeof(Printe));
	}
	else if(Openart_uart_rx_buff[0] ==10)
	{
		memcpy(target_font, keyboard, sizeof(keyboard));
	}
	else if(Openart_uart_rx_buff[0] ==11)
	{
		memcpy(target_font, Mobile_phone, sizeof(Mobile_phone));
	}
		else if(Openart_uart_rx_buff[0] ==12)
	{
		memcpy(target_font, mouse, sizeof(mouse));
	}
	else if(Openart_uart_rx_buff[0] ==13)
	{
		memcpy(target_font, Headphones, sizeof(Headphones));
	}
	else if(Openart_uart_rx_buff[0] ==14)
	{
		memcpy(target_font, Monitor, sizeof(Monitor));
	}
	else if(Openart_uart_rx_buff[0] ==15)
	{
		memcpy(target_font, Speakers, sizeof(Speakers));
	}
//	else if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
//	{
//		//Control_Mode_new_1=0;
//		True_digital = Openart_uart_rx_buff[0] - 16;
//		memset(target_font, 0, sizeof(target_font)); 
//	}
}




void Object_load_new()
{
	switch(Correct_Mode_show)
	{
			case 0:
		//	uint8_t value = Openart_uart_rx_buff[0];
			if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_0=0;
				True_digital_0=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_0=1;
			OpenArt_Data_judje();
			memcpy(object_show_0, target_font, sizeof(target_font));
			}				
			break;
			
			case 1:
					if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_1=0;
				True_digital_1=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_1=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_1, target_font, sizeof(target_font));	
			}
			break;
			
			case 2:
						if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_2=0;
				True_digital_2=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_2=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_2, target_font, sizeof(target_font));
			}				
			break;
			
			case 3:
						if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_3=0;
				True_digital_3=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_3=1;
			
			
			OpenArt_Data_judje();
			memcpy(object_show_3, target_font, sizeof(target_font));	
			}
			break;
			
			case 4:
						if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_4=0;
				True_digital_4=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_4=1;
			
			
			
			OpenArt_Data_judje();
			memcpy(object_show_4, target_font, sizeof(target_font));
			}				
			break;
			
			case 5:
						if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_5=0;
				True_digital_5=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_5=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_5, target_font, sizeof(target_font));	
			}
			break;
			
			case 6:
						if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_6=0;
				True_digital_6=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_6=1;
			
		OpenArt_Data_judje();
			memcpy(object_show_6, target_font, sizeof(target_font));	
			}
			break;
			
			case 7:
					if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_7=0;
				True_digital_7=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_7=1;
			
		OpenArt_Data_judje();
			memcpy(object_show_7, target_font, sizeof(target_font));	
			}
			break;
				case 8:
						if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_8=0;
				True_digital_8=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_8=1;
			
		OpenArt_Data_judje();
			memcpy(object_show_8, target_font, sizeof(target_font));	
			}
			break;
			
			case 9:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_9=0;
				True_digital_9=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_9=1;
			
		OpenArt_Data_judje();
			memcpy(object_show_9, target_font, sizeof(target_font));	
			}
			break;
			case 10:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_10=0;
				True_digital_10=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_10=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_10, target_font, sizeof(target_font));	
			}
			break;
			case 11:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_11=0;
				True_digital_11=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_11=1;
			
		OpenArt_Data_judje();
			memcpy(object_show_11, target_font, sizeof(target_font));	
			}
			break;
			case 12:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_12=0;
				True_digital_12=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_12=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_12, target_font, sizeof(target_font));	
			}
			break;
			case 13:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_13=0;
				True_digital_13=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_13=1;
			
		OpenArt_Data_judje();
			memcpy(object_show_13, target_font, sizeof(target_font));	
			}
			break;
			case 14:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_14=0;
				True_digital_14=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_14=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_14, target_font, sizeof(target_font));	
			}
			break;
			
			case 15:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_15=0;
				True_digital_15=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_15=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_15, target_font, sizeof(target_font));	
			}
			break;
			
			case 16:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_16=0;
				True_digital_16=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_16=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_16, target_font, sizeof(target_font));	
			}
			break;
			
			case 17:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_17=0;
				True_digital_17=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_17=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_17, target_font, sizeof(target_font));	
			}
			break;
			
			case 18:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_18=0;
				True_digital_18=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_18=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_18, target_font, sizeof(target_font));	
			}
			break;
			case 19:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_19=0;
				True_digital_19=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_19=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_19, target_font, sizeof(target_font));	
			}
			break;
			
			case 20:
				if(Openart_uart_rx_buff[0] >= 16 && Openart_uart_rx_buff[0]<= 115)
			{
				Control_Mode_new_20=0;
				True_digital_20=Openart_uart_rx_buff[0]-16;
			}
			else
			{
				Control_Mode_new_20=1;
			
			OpenArt_Data_judje();
			memcpy(object_show_20, target_font, sizeof(target_font));	
			}
			break;
		}
	}
void ips200_show()
{
	
	    ips200_show_chinese(10, 0, 16, serial_number[0], 4, RGB565_RED);
			ips200_show_chinese(110, 0, 16, content[0], 4, RGB565_RED);

}

void ips200_show_0()
{
	switch (Control_Mode_new_0)
	{
		case 0:
				//ips200_show_int(0, 20, 1, 1); 
				ips200_show_int(0, 20, 1, 1); 
				ips200_show_int(60, 20, True_digital_0, 2); 
			break;
		case 1:
				//ips200_show_int(0, 20, 1, 1); 
				ips200_show_int(0, 20, 1, 1); 
				ips200_show_chinese(60, 20, 16, object_show_0[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_1()
{
	switch (Control_Mode_new_1)
	{
		case 0:
	ips200_show_int(0, 40, 2, 1); 
				ips200_show_int(60, 40, True_digital_1, 2); 
			break;
		case 1:
			ips200_show_int(0, 40, 2, 1); 
				ips200_show_chinese(60, 40, 16, object_show_1[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_2()
{
	switch (Control_Mode_new_2)
	{
		case 0:
	ips200_show_int(0, 60, 3, 1); 
				ips200_show_int(60, 60, True_digital_2, 2); 
			break;
		case 1:
			ips200_show_int(0, 60, 3, 1); 
				ips200_show_chinese(60,60, 16, object_show_2[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_3()
{
	switch (Control_Mode_new_3)
	{
		case 0:
	ips200_show_int(0, 80, 4, 1); 
				ips200_show_int(60, 80, True_digital_3, 2); 
			break;
		case 1:
			ips200_show_int(0, 80, 4, 1); 
				ips200_show_chinese(60, 80, 16, object_show_3[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_4()
{
	switch (Control_Mode_new_4)
	{
		case 0:
	ips200_show_int(0, 100,5, 1); 
				ips200_show_int(60, 100, True_digital_4, 2); 
			break;
		case 1:
			ips200_show_int(0, 100,5, 1); 
				ips200_show_chinese(60, 100, 16, object_show_4[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_5()
{
	switch (Control_Mode_new_5)
	{
		case 0:
	ips200_show_int(0, 120,6, 1); 
				ips200_show_int(60, 120, True_digital_5, 2); 
			break;
		case 1:
			ips200_show_int(0, 120,6, 1); 
				ips200_show_chinese(60, 120, 16, object_show_5[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_6()
{
	switch (Control_Mode_new_6)
	{
		case 0:
	ips200_show_int(0, 140,7, 1); 
				ips200_show_int(60, 140, True_digital_6, 2); 
			break;
		case 1:
			ips200_show_int(0, 140,7, 1); 
				ips200_show_chinese(60, 140, 16, object_show_6[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_7()
{
	switch (Control_Mode_new_7)
	{
		case 0:
	ips200_show_int(0, 160, 8, 1); 
				ips200_show_int(60, 160, True_digital_7, 2); 
			break;
		case 1:
			ips200_show_int(0, 160, 8, 1); 
				ips200_show_chinese(60, 160, 16, object_show_7[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_8()
{
	switch (Control_Mode_new_8)
	{
		case 0:
		ips200_show_int(0, 180, 9, 1); 
				ips200_show_int(60, 180, True_digital_8, 2); 
			break;
		case 1:
				ips200_show_int(0, 180, 9, 1); 
				ips200_show_chinese(60, 180, 16, object_show_8[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_9()
{
	switch (Control_Mode_new_9)
	{
		case 0:
		ips200_show_int(0, 200, 10, 2); 
				ips200_show_int(60, 200, True_digital_9, 2); 
			break;
		case 1:
				ips200_show_int(0, 200, 10, 2); 
				ips200_show_chinese(60, 200, 16, object_show_9[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_10()
{
	switch (Control_Mode_new_10)
	{
		case 0:
	ips200_show_int(0, 220, 11, 2); 
				ips200_show_int(60, 220, True_digital_10, 2); 
			break;
		case 1:
			ips200_show_int(0, 220, 11, 2); 
				ips200_show_chinese(60, 220, 16, object_show_10[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_11()
{
	switch (Control_Mode_new_11)
	{
		case 0:
	ips200_show_int(20, 20, 12, 2); 
				ips200_show_int(150, 20, True_digital_11, 2); 
			break;
		case 1:
			ips200_show_int(20, 20, 12, 2); 
				ips200_show_chinese(150, 20, 16, object_show_11[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_12()
{
	switch (Control_Mode_new_12)
	{
		case 0:
	ips200_show_int(20, 40, 13, 2); 
				ips200_show_int(150, 40, True_digital_12, 2); 
			break;
		case 1:
			ips200_show_int(20, 40, 13, 2); 
				ips200_show_chinese(150, 40, 16, object_show_12[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_13()
{
	switch (Control_Mode_new_13)
	{
		case 0:
	ips200_show_int(20, 60, 14, 2); 
				ips200_show_int(150, 60, True_digital_13, 2); 
			break;
		case 1:
			ips200_show_int(20, 60, 14, 2); 
				ips200_show_chinese(150, 60, 16, object_show_13[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_14()
{
	switch (Control_Mode_new_14)
	{
		case 0:
	ips200_show_int(20, 80, 15, 2); 
				ips200_show_int(150, 80, True_digital_14, 2); 
			break;
		case 1:
			ips200_show_int(20, 80, 15, 2); 
				ips200_show_chinese(150, 80, 16, object_show_14[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_15()
{
	switch (Control_Mode_new_15)
	{
		case 0:
	ips200_show_int(20, 100, 16, 2); 
				ips200_show_int(150, 100, True_digital_15, 2); 
			break;
		case 1:
			ips200_show_int(20, 100, 16, 2); 
				ips200_show_chinese(150, 100, 16, object_show_15[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_16()
{
	switch (Control_Mode_new_16)
	{
		case 0:
	ips200_show_int(20, 120, 17, 2); 
				ips200_show_int(150, 120, True_digital_16, 2); 
			break;
		case 1:
			ips200_show_int(20, 120, 17, 2); 
				ips200_show_chinese(150, 120, 16, object_show_16[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_17()
{
	switch (Control_Mode_new_17)
	{
		case 0:
	ips200_show_int(20, 140, 18, 2); 
				ips200_show_int(150, 140, True_digital_17, 2); 
			break;
		case 1:
			ips200_show_int(20, 140, 18, 2); 
				ips200_show_chinese(150, 140, 16, object_show_17[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_18()
{
	switch (Control_Mode_new_18)
	{
		case 0:
	ips200_show_int(20, 160, 19, 2); 
				ips200_show_int(150, 160, True_digital_18, 2); 
			break;
		case 1:
			ips200_show_int(20, 160, 19, 2); 
				ips200_show_chinese(150, 160, 16, object_show_18[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_19()
{
	switch (Control_Mode_new_19)
	{
		case 0:
	ips200_show_int(20, 180, 20, 2); 
				ips200_show_int(150, 180, True_digital_19, 2); 
			break;
		case 1:
			ips200_show_int(20, 180, 20, 2); 
				ips200_show_chinese(150, 180, 16, object_show_19[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_20()
{
	switch (Control_Mode_new_20)
	{
		case 0:
	ips200_show_int(20, 200, 21, 2); 
				ips200_show_int(150, 200, True_digital_20, 2); 
			break;
		case 1:
			ips200_show_int(20, 200, 21, 2); 
				ips200_show_chinese(150, 200, 16, object_show_20[0], 5, RGB565_RED);
			break;
	}
}
void ips200_show_21()
{
	switch (Control_Mode_new_21)
	{
		case 0:
	ips200_show_int(20, 220, 22, 2); 
				ips200_show_int(150, 220, True_digital_21, 2); 
			break;
		case 1:
			ips200_show_int(20, 220, 22, 2); 
				ips200_show_chinese(150, 220, 16, object_show_21[0], 5, RGB565_RED);
			break;
	}
}

void ips200_show_digital()
{
	ips200_show_int(0, 20, 1, 1); 
	ips200_show_int(0, 40, 2, 1); 
	ips200_show_int(0, 60, 3, 1); 
	ips200_show_int(0, 80, 4, 1); 
	ips200_show_int(0, 100,5, 1); 
	ips200_show_int(0, 120,6, 1); 
	ips200_show_int(0, 140,7, 1); 
	ips200_show_int(0, 160, 8, 1); 
	ips200_show_int(0, 180, 9, 1); 
	ips200_show_int(0, 200, 10, 2); 
	ips200_show_int(0, 220, 11, 2); 
	ips200_show_int(20, 20, 12, 2); 
	ips200_show_int(20, 40, 13, 2); 
	ips200_show_int(20, 60, 14, 2); 
	ips200_show_int(20, 80, 15, 2); 
}
void ips200_show_object_all()
{
	ips200_show_0();
	ips200_show_1();
	ips200_show_2();
	ips200_show_3();
	ips200_show_4();
	ips200_show_5();
	ips200_show_6();
	ips200_show_7();
	ips200_show_8();
	ips200_show_9();
	ips200_show_10();
	ips200_show_11();
	ips200_show_12();
	ips200_show_13();
	ips200_show_14();
	ips200_show_15();
	ips200_show_16();
	ips200_show_17();
	ips200_show_18();
	ips200_show_19();
	ips200_show_20();
	ips200_show_21();
//	ips200_show_14();
	
}
//void ips200_show_sign()
//	
//{
//	
//	ips200_show_chinese(160, 220, 16, content[0], 4, RGB565_RED);
//	
//	
//}
//void ips200_show_digital_new()
//{
//	ips200_show_int(40, 20, True_digital_8, 3); 
//	ips200_show_int(40, 40, True_digital_8, 3); 
//	ips200_show_int(40, 60, True_digital_8, 3); 
//	ips200_show_int(40, 80, True_digital_8, 3); 
//	ips200_show_int(40, 100,True_digital_8, 3); 
//	ips200_show_int(40, 120,True_digital_8, 3); 
//	ips200_show_int(40, 140,True_digital_8, 3); 
//	ips200_show_int(40, 160, True_digital_8, 3); 
//	ips200_show_int(40, 180, True_digital_8, 3); 
//	ips200_show_int(40, 200, True_digital_8, 3); 
//	ips200_show_int(40, 220, True_digital_8, 3); 
//}






//	void ips200_show_1()
//{
//	  ips200_show_int(20, 40, True_digital, 3);  
//}
//	const uint8_t (*OpenArt_Data_judje(void))[16] 
//{
//    if(Openart_uart_rx_buff[0] == 1)  // 注意：这里应该是 == 而不是 =
//    {
//        return mouse;  // 返回数组指针
//    }
//    else {
//        return NULL;  // 或返回其他数组指针
//    }
//}
//	void iow()
//	{
//	mouse_s=const uint8_t (*OpenArt_Data_judje(void))[16] ;
//	
//	}
//	// 1. 定义全局变量
//uint8_t target_font[20][16];      // 用于存储工具点阵数据
//uint8_t True_digital = 0;        // 存储数字值20
//bool show_tool = false;          // 是否显示工具
//bool show_digit = false;         // 是否显示数字

//// 2. 修改判断函数
//void OpenArt_Data_judje()
//{
//    uint8_t value = Openart_uart_rx_buff[0];
//    
//    // 重置显示状态
//    show_tool = false;
//    show_digit = false;    
//    // 处理工具类型 (1-15)
//    if(value >= 1 && value <= 15) {
//        show_tool = true;
//        switch(value) {
//            case 1: memcpy(target_font, Wrench, sizeof(Wrench)); break;
//            case 2: memcpy(target_font, Soldering_iron, sizeof(Soldering_iron)); break;
//            case 3: memcpy(target_font, Electric_drill, sizeof(Electric_drill)); break;
//            case 4: memcpy(target_font, Tape_measure, sizeof(Tape_measure)); break;
//            case 5: memcpy(target_font, Screwdriver, sizeof(Screwdriver)); break;
//            case 6: memcpy(target_font, Pliers, sizeof(Pliers)); break;
//            case 7: memcpy(target_font, Oscilloscope, sizeof(Oscilloscope)); break;
//            case 8: memcpy(target_font, Multimeter, sizeof(Multimeter)); break;
//            case 9: memcpy(target_font, Printe, sizeof(Printe)); break;
//            case 10: memcpy(target_font, keyboard, sizeof(keyboard)); break;
//            case 11: memcpy(target_font, Mobile_phone, sizeof(Mobile_phone)); break;
//            case 12: memcpy(target_font, mouse, sizeof(mouse)); break;
//            case 13: memcpy(target_font, Headphones, sizeof(Headphones)); break;
//            case 14: memcpy(target_font, Monitor, sizeof(Monitor)); break;
//            case 15: memcpy(target_font, Speakers, sizeof(Speakers)); break;
//        }
//    }
//    // 处理数字 (16-115)
//    else if(value >= 16 && value <= 115) {
//        show_digit = true;
//        True_digital = value - 16;
//    }
//}

//// 3. 显示函数 - 在屏幕不同区域显示工具和数字
//void display_selected()
//{
////    // 清屏或清空相关区域
////    LCD_ClearArea(TOOL_X, TOOL_Y, TOOL_WIDTH, TOOL_HEIGHT);  // 工具区域
////    LCD_ClearArea(DIGIT_X, DIGIT_Y, DIGIT_WIDTH, DIGIT_HEIGHT); // 数字区域
//    
//    // 显示工具图标（如果检测到工具）
//    if(show_tool) 
//			{
////        for(int y = 0; y < 8; y++) {
////            for(int x = 0; x < 16; x += 2) {
////                uint16_t line = (target_font[y][x] << 8) | target_font[y][x+1];
////                
////                // 绘制工具图标
////                for(int bit = 0; bit < 16; bit++) {
////                    if(line & (1 << (15 - bit))) {
////                        LCD_DrawPixel(TOOL_X + x*8 + bit, TOOL_Y + y, BLACK);
////                    }
////                }
////            }
////        }
//			
//			
//			
//			
//    }
//    
//    // 显示数字（如果检测到数字）
//    if(show_digit) {
//        // 显示数字值
//        char digit_str[4];
//        snprintf(digit_str, sizeof(digit_str), "%d", True_digital);
//        LCD_DisplayString(DIGIT_X, DIGIT_Y, digit_str);
//        
//        // 或者使用点阵字体显示数字
//        // display_digit_font(True_digital, DIGIT_X, DIGIT_Y);
//    }
//}

