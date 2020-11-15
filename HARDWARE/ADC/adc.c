

#include "delay.h"

#include "adc.h"
#include "stm32f4xx_dma.h"
/*********************************************************************************

**********************************************************************************
*********************************************************************************/

__IO uint16_t ADC_ConvertedValue[5];//ADCͨ������10�β�������

//��ʼ��ADC				
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
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);// ��DMAʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE);
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0 ;  //ͨ��ѡ��
  	DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)ADC1_BASE+0x4c);//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = 5;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�е����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//��ʼ��DMA Stream
	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);



  //�ȳ�ʼ��ADC1ͨ��6 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PA6  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2;//PC0\2\3 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	
	
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//	ɨ��ģʽ     ��ͨ����ɨ��ģʽ����ͨ������ɨ��ģʽ					Ҳ����˵��ͨ��ENABLE,��ͨ��DISABLE
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��,����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 4;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_480Cycles );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_480Cycles );	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_480Cycles );	



	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
	ADC_SoftwareStartConv(ADC1);  
	


///////F1ϵ����ҪADCУ׼��F4ϵ��û˵/////

//	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
//	
//	ADC_StartCalibration(ADC1);	 //����ADУ׼
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}			
*/
/****************************************************************************
* ��    ��: u16 Get_Adc(u8 ch) 
* ��    �ܣ����ADCֵ
* ��ڲ�����ch: ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
* ���ز�����12λADC��Чֵ
* ˵    ����       
****************************************************************************/

void  ADC1_Init(void)
{    
  GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
//	
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC2ʱ��

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PC0\2\3 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��  
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_2;//PC0\2\3 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
 
	ADC_TempSensorVrefintCmd(ENABLE);

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//	ɨ��ģʽ     ��ͨ����ɨ��ģʽ����ͨ������ɨ��ģʽ					Ҳ����˵��ͨ��ENABLE,��ͨ��DISABLE
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��,����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��

		ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_480Cycles );

	ADC_Cmd(ADC1, ENABLE);//����ADת����	
	
	
	

///////F1ϵ����ҪADCУ׼��F4ϵ��û˵/////

//	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
//	
//	ADC_StartCalibration(ADC1);	 //����ADУ׼
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}			



u16 Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    

	ADC_SoftwareStartConv(ADC1);		  //ʹ��ָ����ADC1�����ת����������	

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )){}		//�ȴ�ת������

	
	return ADC_GetConversionValue(ADC1);	 //�������һ��ADC1�������ת�����
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
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);// ��DMAʱ��
//		DMA_DeInit(DMA2_Stream0);

//  DMA_InitStructure.DMA_Channel = DMA_Channel_0 ;  //ͨ��ѡ��
//  	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );//DMA�����ַ
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;;//DMA �洢��0��ַ
//  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
//  DMA_InitStructure.DMA_BufferSize = 3*10;//���ݴ����� 
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
//  DMA_InitStructure.DMA_PeripheralDataSize =  DMA_PeripheralDataSize_HalfWord;//�������ݳ���:8λ
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ���:8λ
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ����ͨģʽ 
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�е����ȼ�
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
//  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//��ʼ��DMA Stream
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


