#ifndef __LED_H
#define __LED_H
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////////	

//LED�˿ڶ���
#define LED0 PEout(3)	 
#define LED1 PEout(4)	 
#define LED2 PGout(9)	  
#define BEEP PGout(7)	// ����������IO 

//��������
void BEEP_Init(void); //��ʼ��	
//��������
void LED_Init(void);//��ʼ��	

#endif
