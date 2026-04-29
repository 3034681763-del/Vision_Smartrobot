#include "zf_common_headfile.h"

uint8 MCX_uart_rx_buff[5];
uint8 Openart_uart_rx_buff[2];
uint8 Openart_uart_transform[8];
OPENARTMINI_GET OPENART;

void OpenArtmini_init()
{
	uart_init(OPENART_UART_INDEX, OPENART_UART_BAUDRATE, OPENART_UART_TX_PIN, OPENART_UART_RX_PIN);
	uart_rx_interrupt(OPENART_UART_INDEX, ZF_ENABLE);
	interrupt_set_priority(OPENART_UART_PRIORITY, 0);
}

void MCX_NEW_init()
{
	uart_init(MCX_UART_INDEX, MCX_UART_BAUDRATE, MCX_UART_TX_PIN, MCX_UART_RX_PIN);    
	uart_rx_interrupt(MCX_UART_INDEX, ZF_ENABLE);                                      
	interrupt_set_priority(MCX_UART_PRIORITY, 0);                                     
}

void OpenArtmini_uart_rx_interrupt_handler()
{
	static uint8_t Rx_State = 0;
	static uint8_t pRx_Packet = 0;
	uint8_t Openart_Rx_Data = uart_read_byte(OPENART_UART_INDEX); //读取串口接收到的数据
	
	if(Rx_State == 0)
	{
		if(Openart_Rx_Data == 0xAF)
		{
			Rx_State = 1;
			pRx_Packet = 0;
		}
	}
	else if(Rx_State == 1)      
	{
		Openart_uart_rx_buff[pRx_Packet] = Openart_Rx_Data;
		OpenArt_Data_Transform(Openart_uart_rx_buff[0]);
		pRx_Packet++;
		if(pRx_Packet >= 2)
		{
			Rx_State = 2;
		}
	}
	else if(Rx_State == 2)      
	{
		if(Openart_Rx_Data == 0xFA)
		{
			Rx_State = 0;
			pRx_Packet = 0;
		}
	}
}

void MCX_NEW_uart_rx_interrupt_handler()
{
	static uint8_t Rx_State = 0;
	static uint8_t pRx_Packet = 0;
	uint8_t MCX_Rx_Data = uart_read_byte(MCX_UART_INDEX); //读取串口接收到的数据
	
	if(Rx_State == 0)
	{
		if(MCX_Rx_Data == 0xAA) 
		{
			Rx_State = 1;
			pRx_Packet = 0;
		}
	}
	else if(Rx_State == 1)      
	{
		MCX_uart_rx_buff[pRx_Packet] = MCX_Rx_Data;
		pRx_Packet++;
		if(pRx_Packet >= 4)
		{
			Rx_State = 2;
		}
	}
	else if(Rx_State == 2)      
	{
		if(MCX_Rx_Data == 0xFF)
		{
			Rx_State = 0;
			pRx_Packet = 0;
		}
	}
}


static void OpenArt_Data_Transform(uint8 ART_DETECTION_RESULT)
{
	if(is_rotating)
		{
			return;
		}
	
	if(ART_DETECTION_RESULT >= 1 && ART_DETECTION_RESULT <= 15)
	{
		OPENART.kind = 1;
		OPENART.label = ART_DETECTION_RESULT;
		OPENART.broad_heading_15 = push_dir(ART_DETECTION_RESULT);
	}
	
	else if(ART_DETECTION_RESULT >= 16 && ART_DETECTION_RESULT <= 115)
	{
		OPENART.kind = 2;
		OPENART.label = ART_DETECTION_RESULT - 16;
		OPENART.broad_heading_num = push_dir(ART_DETECTION_RESULT);
	}
}

uint8 push_dir(uint8 ART_DETECTION_RESULT)
{
	
	
	static uint8 num_class;
	
	
	if(ART_DETECTION_RESULT >= 1 && ART_DETECTION_RESULT <= 15)
	{
		
		if(ART_DETECTION_RESULT >= 1 && ART_DETECTION_RESULT <= 8)
		{
			return 1;
		}
		
		else if(ART_DETECTION_RESULT >= 9 && ART_DETECTION_RESULT <= 15)
		{
			return 2;
		}
	}
	
	else if(ART_DETECTION_RESULT >= 16 && ART_DETECTION_RESULT <= 115)
	{
		num_class = (ART_DETECTION_RESULT - 16) % 2;
		
		if(num_class == 0)
		{
			return 1;
		}
		
		if(num_class == 1)
		{
			return 2;
		}
	}
	return 0;
}

void Jugde_Direction()
{
	
		
	if(OPENART.kind == 1)
	{
		
		Revolove_Box(OPENART.broad_heading_15);
//		ips200_show_string();
	}
	else if(OPENART.kind == 2)
	{
		
		Revolove_Box(OPENART.broad_heading_num);
//		ips200_show_string();
	}
	
}
	
void my_camera_init()
{
	MCX_NEW_init();
	OpenArtmini_init();
}




