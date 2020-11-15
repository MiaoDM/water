

#include "delay.h"

#include "adc.h"
#include "stm32f4xx_dma.h"
/*********************************************************************************

**********************************************************************************
*********************************************************************************/

__IO uint16_t ADC_ConvertedValue[5];//ADC通道数，10次采样次数

//初始化ADC				
/*

VREFINT 
*/
//# VREF_CAL;
/*void  ADC1_Init(void)
{   
	DMA_InitTypeDef      DMA_InitStructure; 
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	ADC_InitTypeDef       ADC_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);// 打开DMA时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE);
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0 ;  //通道选择
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)ADC1_BASE+0x4c);//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = 5;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);



  //先初始化ADC1通道6 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2;//PC0\2\3 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
	
	
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//	扫描模式     多通道：扫描模式；单通道：非扫描模式					也就是说多通道ENABLE,单通道DISABLE
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//连续转换,单次转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 4;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_480Cycles );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_480Cycles );	



	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	ADC_SoftwareStartConv(ADC1);  
	


///////F1系列需要ADC校准，F4系列没说/////

//	ADC_ResetCalibration(ADC1);	//使能复位校准  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
//	
//	ADC_StartCalibration(ADC1);	 //开启AD校准
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}			
*/
/****************************************************************************
* 名    称: u16 Get_Adc(u8 ch) 
* 功    能：获得ADC值
* 入口参数：ch: 通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
* 返回参数：12位ADC有效值
* 说    明：       
****************************************************************************/

void  ADC1_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
//	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC2时钟

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PC0\2\3 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_2;//PC0\2\3 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
	

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	ADC_TempSensorVrefintCmd(ENABLE);

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//	扫描模式     多通道：扫描模式；单通道：非扫描模式					也就是说多通道ENABLE,单通道DISABLE
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换,单次转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化

		ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_480Cycles );

	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	
	
	

///////F1系列需要ADC校准，F4系列没说/////

//	ADC_ResetCalibration(ADC1);	//使能复位校准  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
//	
//	ADC_StartCalibration(ADC1);	 //开启AD校准
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
}			



u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    

	ADC_SoftwareStartConv(ADC1);		  //使能指定的ADC1的软件转换启动功能	

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){}		//等待转换结束

	
	return ADC_GetConversionValue(ADC1);	 //返回最近一次ADC1规则组的转换结果
}


u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
//		temp_val+=3*(Get_Adc(ch)*1669/(Get_Adc(ADC_Channel_17)*4096));
			
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 	 


//void DMA1_Channel1_IRQHandler(void)
//{
// if(DMA_GetITStatus(DMA1_IT_TC1))
//  {
//  DMA_ClearITPendingBit(DMA1_IT_GL1); //????????
//  ADC_Ok=TRUE;
// }
//}
//void DMACC_Init(void)
//{
//	DMA_InitTypeDef DMA_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);// 打开DMA时钟
//		DMA_DeInit(DMA2_Stream0);

//  DMA_InitStructure.DMA_Channel = DMA_Channel_0 ;  //通道选择
//  	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );//DMA外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;;//DMA 存储器0地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
//  DMA_InitStructure.DMA_BufferSize = 3*10;//数据传输量 
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
//  DMA_InitStructure.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord;//外设数据长度:8位
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:8位
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用普通模式 
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//中等优先级
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
//  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream
//	DMA_Cmd(DMA2_Stream0, ENABLE);
//	
//}


//u16 ReadADCAverageValue(uint16_t  Channel)
//{
//    uint8_t i,Vchx;
//    u32 sum = 0;
//	
//		
//    for(i=0; i<Sample_Num; i++)
//    {
//     sum+=ADC_ConvertedValue[i][Channel];
//    }
//	
//		Vchx=(sum/Sample_Num);		
//	
//		
//    return (Vchx);
//}


