#ifndef _MCX_H_
#define _MCX_H_
#include "zf_common_headfile.h"

/***************** 宏变量 *****************/
#define MCX_UART_INDEX              (UART_4   )                             
#define MCX_UART_BAUDRATE           (115200)                                
#define MCX_UART_TX_PIN             (UART4_TX_C16  )                        
#define MCX_UART_RX_PIN             (UART4_RX_C17  )                        
#define MCX_UART_PRIORITY           (LPUART4_IRQn)  

#define OPENART_UART_INDEX              (UART_1   )                             
#define OPENART_UART_BAUDRATE           (115200)                                
#define OPENART_UART_TX_PIN             (UART1_TX_B12  )                        
#define OPENART_UART_RX_PIN             (UART1_RX_B13  )                        
#define OPENART_UART_PRIORITY           (LPUART1_IRQn)  


/******************* 结构体 *******************/
typedef struct
{
	uint16 x1;          //色块的中心横坐标
    uint16 y1;          //色块的中心纵坐标
    uint16 trace_rate;  //找到的矩形所占的比例
}MCX_GET;
//extern MCX_GET mcx_get;
//extern uint16 MCX_GET.trace_rate;
typedef struct
{
	uint16 kind;         		//普通分类-1 手写数字-2
    uint16 label;        		//对应分类的标签序号
    uint16 broad_heading_15; 	//普通分类的推箱子方向
	uint16 broad_heading_num;	//手写数字的推箱子方向
}OPENARTMINI_GET;

extern OPENARTMINI_GET OPENART; // 声明全局结构体
/******************* 变量 *******************/
extern uint8 MCX_uart_rx_buff[5];
extern uint8 Openart_uart_rx_buff[2];
extern bool is_rotating;
extern uint8_t Rx_State;
/******************* 函数 *******************/
void MCX_uart_rx_interrupt_handler();
void MCX_NEW_init();
void my_camera_init();
void OpenArtmini_uart_rx_interrupt_handler();
void OpenArtmini_init();
void Revolove_Box(uint8 dir);
static void OpenArt_Data_Transform(uint8 ART_DETECTION_RESULT);
uint8 push_dir(uint8 art_detection_result);
void Jugde_Direction();	//得到推箱子方向


#endif