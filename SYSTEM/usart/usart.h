#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.csom
//修改日期:2011/6/14
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	
//#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
#define BUFLEN 256      //数组缓存大小
typedef struct _UART_BUF
{
    char buf [BUFLEN+1];               
    unsigned int index ;
}UART_BUF;


#define UART5_MAX_RECV_LEN		400					//最大接收缓存字节数
#define UART5_MAX_SEND_LEN		400					//最大发送缓存字节数
#define UART5_RX_EN 			1					//0,不接收;1,接收.

#define USART3_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		400					//最大发送缓存字节数
#define USART3_RX_EN 			1					//0,不接收;1,接收.

extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//接收缓冲,最大USART4_MAX_RECV_LEN字节
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//发送缓冲,最大USART4_MAX_SEND_LEN字节
extern u16 UART5_RX_STA;   						//接收数据状态
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//发送缓冲,最大USART3_MAX_SEND_LEN字节
extern u16 USART3_RX_STA;   						//接收数据状态


void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);
 void u5_printf(char* fmt,...);


void uart1_init(u32 bound);     //串口初始化
void uart2_init(u32 bound);
void uart3_init(u32 bound);				//串口3初始化 
void uart5_init(u32 bound);				//串口4初始化


void Uart1_SendStr(char*SendBuf);   //字符串发送
void Uart2_SendStr(char*SendBuf);



extern UART_BUF buf_uart1;     //CH340
extern UART_BUF buf_uart2;     //NBIOT
//extern UART_BUF buf_uart3;     //TTL

//如果想串口中断接收，请不要注释以下宏定义




#endif


