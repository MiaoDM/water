#ifndef __ADC_H
#define __ADC_H	
#include "stm32f4xx.h"
#include "sys.h"
 	
#define  Channel_Num  3 //ADC????,????9???
#define  Sample_Num  10 //????,????????,??10????


void  ADC2_Init(void);
void  ADC1_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);
// void DMACC_Init(void);
u16 ReadADCAverageValue(uint16_t  Channel);
 
#endif 


