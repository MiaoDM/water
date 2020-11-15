#ifndef __LED_H
#define __LED_H
#include "sys.h"

////////////////////////////////////////////////////////////////////////////////////	

//LED端口定义
#define LED0 PEout(3)	 
#define LED1 PEout(4)	 
#define LED2 PGout(9)	  
#define BEEP PGout(7)	// 蜂鸣器控制IO 

//函数声明
void BEEP_Init(void); //初始化	
//函数声明
void LED_Init(void);//初始化	

#endif
