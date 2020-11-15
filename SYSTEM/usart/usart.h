#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2011/6/14
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
//#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
#define BUFLEN 256      //���黺���С
typedef struct _UART_BUF
{
    char buf [BUFLEN+1];               
    unsigned int index ;
}UART_BUF;


#define UART5_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define UART5_RX_EN 			1					//0,������;1,����.

#define USART3_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		400					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���USART4_MAX_RECV_LEN�ֽ�
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���USART4_MAX_SEND_LEN�ֽ�
extern u16 UART5_RX_STA;   						//��������״̬
extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   						//��������״̬


void TIM7_Int_Init(u16 arr,u16 psc);
void u3_printf(char* fmt, ...);
 void u5_printf(char* fmt,...);


void uart1_init(u32 bound);     //���ڳ�ʼ��
void uart2_init(u32 bound);
void uart3_init(u32 bound);				//����3��ʼ�� 
void uart5_init(u32 bound);				//����4��ʼ��


void Uart1_SendStr(char*SendBuf);   //�ַ�������
void Uart2_SendStr(char*SendBuf);



extern UART_BUF buf_uart1;     //CH340
extern UART_BUF buf_uart2;     //NBIOT
//extern UART_BUF buf_uart3;     //TTL

//����봮���жϽ��գ��벻Ҫע�����º궨��




#endif


