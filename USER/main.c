#include "sys.h"
#include "adc.h"
#include "delay.h"
#include "usart.h"
#include "bc28.h"
#include "led.h"
#include "lcd.h"
#include "key.h"
#include "timer.h"
//#include "ds18b20.h"
#include "HEXSTR.h"
#include "math.h"	
#include "gps.h"

#include "hc05.h" 	


#define RES2 820.0   //�˷ŵ��裬��Ӳ����·�й�
#define ECREF 200.0 //��Ƭ�����ѹ����Ӳ����·���
void Gps_Msg_Show(void);
void EC_Value_Conversion(void);
void TU_Value_Conversion(void);
void HC05_Role_Show(void);
GPIO_InitTypeDef GPIO_InitStructure;
static __IO uint32_t TimingDelay;


extern int err;
u8 temp=15,humi;


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[5];

// ���ڱ���ת�������ĵ�ѹֵ 	 
u16 ADC_Value;
double compensationVolatge;


//**************************************GPS������Ϣ****************************************************************//
u8 USART1_TX_BUF[USART3_MAX_RECV_LEN]; 									//����1,���ͻ�����
nmea_msg gpsx; 																					//GPS��Ϣ
__align(4) u8 dtbuf[50];   															//��ӡ������
__align(4) u8 dtbuf1[50]; 
__align(4) u8 dtbuf2[50]; 
__align(4) u8 dtbuf3[50]; 
u8 Longitude1,Latitude1,Altitude1;
float tp;	


//***********************************************�¶�***********************************************************//
float temp_data=255;
/////**********************************************���Ե��Գ������***********************************************////
//u8 Temperature=15;
u16 ADC_Value_adj, ADC_Value2;

//***********************************************PHֵģ�����**********************************************/
float PH_Value=0,PH_GET,mmm;

//***********************************************�絼��ֵģ�����**********************************************/
float EC_Value,EC_GET;
float raw_EC=0.0;
//double kValue=1.67;
float kValue=1.0;
float kValue_Low=1150.48;  //У׼ʱ�����޸�
float kValue_High=1282.5; //У׼ʱ�����޸�// 
float EC_valueTemp=0.0;
float EC_value=0.0;
float compensationCoefficient=1.0;//�¶�У׼ϵ��
//**********************************************TDSֵģ�����**********************************************/

float TDS_Value,TDS_GET;
float kValue_TDS=1.67;//1.67&&0.9

//***********************************************TU���Ƕȣ�ֵ����**********************************************/
float K_Value_TU=3047.19;//У׼ʱ�����޸�// 
float TU=0.0;
float TU_value=0.0;
float TU_calibration=0.0;



//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
int main(void)
{
    short temperature;
	  char senddata_PH[BUFLEN];
		char senddata_EC[BUFLEN];
		char senddata_TDS[BUFLEN];
    char tempstr[BUFLEN];

		char tempstr_GPS[BUFLEN];
	  char senddata_GPS[BUFLEN];
	
		u8 display_ph[4];
		u8 key=0XFF;
		u8 key_HC05=0;
		u16 i,rxlen;
		u16 lenx,c=10;
		u8 upload=0; 
	if (SysTick_Config(SystemCoreClock / 1000))//����24bit��ʱ�� 1ms�ж�һ��
    {
        /* Capture error */
        while (1);
    }			
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);
		
	
    LED_Init();
		LCD_Init(); 
    uart1_init(9600);
    uart2_init(9600);
    uart3_init(38400);

//    ADC1_Init();
		ADC1_Init();

//		LCD_DisplayString(50,110,8,"KEY0:Screen Test");	

		
    Uart1_SendStr("init stm32L COM1 \r\n"); //��ӡ��Ϣ
//Uart2_SendStr("init stm32L COM2 \r\n"); //��NBIOT����

//		HC05_Init();
		 	TIM3_Int_Init(4999,8399);
		if(SkyTra_Cfg_Rate(5)!=0)	//���ö�λ��Ϣ�����ٶ�Ϊ5Hz,˳���ж�GPSģ���Ƿ���λ. 
	{
		do
		{
			uart3_init(9600);			//��ʼ������3������Ϊ9600
	  	SkyTra_Cfg_Prt(3);			//��������ģ��Ĳ�����Ϊ38400
			uart3_init(38400);			//��ʼ������3������Ϊ38400
      key=SkyTra_Cfg_Tp(100000);	//������Ϊ100ms
		}while(SkyTra_Cfg_Rate(5)!=0&&key!=0);//����SkyTraF8-BD�ĸ�������Ϊ5Hz
	
	}

		LCD_DisplayString(30,80,24,"Wainting Init...");	
    while(BC28_Init()); 	
    BC28_PDPACT();
		BC28_RegALIYUNIOT();//ע�ᵽ�����ƻ�Ϊ��ƽ̨

////////
// 	while(DS18B20_Init())	//DS18B20��ʼ��	
//	{
//		LCD_DisplayString(30,230,16,"DS18B20 Error");
//		delay_ms(200);
//		LCD_Fill_onecolor(30,230,239,130+16,WHITE);
// 		delay_ms(200);
//	}   
//		LCD_DisplayString(30,230,16,"DS18B20 OK");
//		temperature=DS18B20_Get_Temp();  //�ȶ�ȡһ���¶�ֵ
//	
//	BRUSH_COLOR=BLUE; //��������Ϊ��ɫ 
// 	LCD_DisplayString(30,250,16,"Temp:   . C");	 
BRUSH_COLOR=GREEN;
	
	LCD_Fill_onecolor(30,80,250,100,WHITE);
	LCD_DisplayString(30,120,16,"Date��");	


 while (1)
 {
//		key_HC05=HC05_Get_Role();
//			if(key_HC05!=0XFF)
//			{
//				key_HC05=!key_HC05;  					//״̬ȡ��	   
//				if(key_HC05==0)HC05_Set_Cmd("AT+ROLE=0");
//				else HC05_Set_Cmd("AT+ROLE=1");
//				HC05_Role_Show();
//				HC05_Set_Cmd("AT+RESET");	//��λATK-HC05ģ��
//				delay_ms(200);
//			}
//	//**************************************************** 
	 
	 if(USART3_RX_STA&0X8000)		//���յ�һ��������
		{
			rxlen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
			for(i=0;i<rxlen;i++)USART1_TX_BUF[i]=USART3_RX_BUF[i];	   
 			USART3_RX_STA=0;		   	//������һ�ν���
			USART1_TX_BUF[i]=0;			//�Զ���ӽ�����
			GPS_Analysis(&gpsx,(u8*)USART1_TX_BUF);//�����ַ���
			Gps_Msg_Show();				//��ʾ��Ϣ	
		  printf("\r\n%s\r\n",USART1_TX_BUF);//���ͽ��յ������ݵ�����1
 		}
		
	if(tp!=0)
	{	
		
		if(c==10)
		{
			BC28_ALYIOTSenddata3((u8 *)"32",(u8 *)dtbuf1,(u8 *)dtbuf2,(u8 *)dtbuf3);
		c=1;
		}
		
	}
			BRUSH_COLOR=BLACK;

	 //temperature=DS18B20_Get_Temp();	
//*************************************************************ADC���У׼********************************************************************//  
		ADC_Value_adj=Get_Adc_Average(ADC_Channel_17,10);										//�ڲ��ο���ѹ��У׼ͨ��
		ADC_Value= Get_Adc_Average(ADC_Channel_10,10);
	  mmm=1.21*Get_Adc_Average(ADC_Channel_10,10)/ADC_Value_adj;

//*************************************************************PH���*************************************************************************// 
		
	  PH_GET=1.21*(float)Get_Adc_Average(ADC_Channel_6,10)/ADC_Value_adj; //ADת��
	  PH_Value=-5.7541*PH_GET+16.654;
	 	if(PH_Value<=0.0){PH_Value=0.0;}
		if(PH_Value>=14.0){PH_Value=14.0;}
		sprintf((char*)display_ph,"PH:%.2f ",PH_Value); 
//		LCD_DisplayNum(85,150,PH_Value,4,16,0);
		LCD_DisplayString(50,150,16,display_ph);
		
		if(PH_Value<=6.0||PH_Value>=9.5)
			{
				BRUSH_COLOR=RED;
			LCD_DisplayString(140,150,16,"!WARNING!");
					PGout(7)=1;
				delay_ms(200);
			LCD_Fill_onecolor(140,150,220,167,WHITE);
			}
			else{
			
			LCD_DisplayString(140,150,16," ");
			}
		BRUSH_COLOR=BLACK;
		
		memset(tempstr,0,BUFLEN);
    memset(senddata_PH,0,BUFLEN);
    sprintf(tempstr,"%f",PH_Value);
    strcat(senddata_PH,tempstr);
//     strcat(senddata_PH,".0");
    printf("senddata = %s \r\n",senddata_PH);
	  BC28_ALYIOTSenddata1((u8 *)"32",(u8 *)senddata_PH);//�����ݵ�������ƽ̨
//		u5_printf("senddata = %s \r\n",senddata_PH);
//***********�絼�ʼ��****************************//
		
		
		EC_Value_Conversion();
		sprintf((char*)display_ph,"EC:%f mS/cm",EC_value);
		LCD_DisplayString(50,170,16,display_ph);
		memset(tempstr,0,BUFLEN);
    memset(senddata_EC,0,BUFLEN);
    sprintf(tempstr,"%f",EC_value);
    strcat(senddata_EC,tempstr);												//1000*EC_value,���͵�����������us/cm
		BC28_ALYIOTSenddata2((u8 *)"32",(u8 *)senddata_EC,(u8 *)"TargetEC");//�����ݵ�������ƽ̨
//***********TDS���*********************************//		
		
		
		TDS_GET=EC_GET; //ADת��
		compensationCoefficient=1.0+0.02*((temp_data/10)-25.0); 
		compensationVolatge=TDS_GET/compensationCoefficient;
		if((TDS_GET>=0)&&(TDS_GET<0.1))
		{
			compensationVolatge=0;
		}
		TDS_Value=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5*kValue_TDS;
	
	  if((TDS_Value<=0)){TDS_Value=0;}
		if((TDS_Value>1400)){TDS_Value=1400;}	
		
		sprintf((char*)display_ph,"TDS:%f ppm",TDS_Value);
		LCD_DisplayString(50,190,16,display_ph);
		memset(tempstr,0,BUFLEN);
    memset(senddata_TDS,0,BUFLEN);
    sprintf(tempstr,"%f",TDS_Value);
    strcat(senddata_TDS,tempstr);	
		BC28_ALYIOTSenddata2((u8 *)"32",(u8 *)senddata_TDS,(u8 *)"TDS");//�����ݵ�������ƽ̨
//*****************�Ƕȼ��*******************************//
		TU_Value_Conversion();
		sprintf((char*)display_ph,"TU:%f NTU",TU_value);
		LCD_DisplayString(50,210,16,display_ph);
	
	

//**********************���ڴ�ӡ����**********************//
			
			printf("ADC17:%d\r\n",ADC_Value_adj);
//			printf("%d\r\n",temperature);
			printf("V��%f RH\r\n",mmm);
			printf("PH��%f C \r\n",PH_Value);
			printf("PH_GET��%f RH\r\n",PH_GET);
			printf("EC��%f mS/cm\r\n",EC_value);
			printf("EC_GET��%f RH\r\n",EC_GET);
//			printf("kValue��%f \r\n",kValue);
			printf("TDS��%f ppm\r\n",TU_value);
			printf("TU��%f NTU\r\n",TU_value);
			
      GPIO_ToggleBits(GPIOE,GPIO_Pin_3);//��תLED  


    }
}

//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
//************************************************************************************************************************************************************************************/
//*************************************************************�絼�ʼ���ӳ���***********************************&**************//

void EC_Value_Conversion()
{
	
	EC_GET=1.21*(float)Get_Adc_Average(ADC_Channel_10,10)/ADC_Value_adj; //ADת��
	raw_EC = 1000*EC_GET/RES2/ECREF;
	EC_valueTemp=raw_EC*kValue;
	
		/*First Range:(0,2); Second Range:(2,20)*/
	if(EC_valueTemp>2.0)
	{
	  kValue=kValue_High;
	}
	else if(EC_valueTemp<=2.0)
	{
	  kValue=kValue_Low;
	}
		EC_value=raw_EC*kValue;
	compensationCoefficient=1.0+0.0185*((temp_data/10)-25.0); //�¶�ϵ������
	EC_value=EC_value/compensationCoefficient;


}

//*************************************************************�Ƕȼ���ӳ���******************************************************************//
void TU_Value_Conversion()
{
		TU =1.21*(float)Get_Adc_Average(ADC_Channel_12,10)/ADC_Value_adj; //ADת��
	
		TU_calibration=-0.0192*(temp_data/10-25)+TU;  
	  TU_value=-865.68*TU_calibration + K_Value_TU;
	
		if(TU_value<=0){TU_value=0;}
		if(TU_value>=3000){TU_value=3000;}


}


	  
//*************************************************************//��ʾGPS��λ��Ϣ *******************************************************************//
void Gps_Msg_Show(void)
{
 		   
	BRUSH_COLOR=BLUE;  	 
	tp=gpsx.longitude;	   
	sprintf((char *)dtbuf1,"Longitude:%.5f %1c   ",tp/=100000,gpsx.ewhemi);	//�õ������ַ���

 	LCD_DisplayString(50,20,16,dtbuf1);	 	   
	tp=gpsx.latitude;	   
	sprintf((char *)dtbuf2,"Latitude:%.5f %1c   ",tp/=100000,gpsx.nshemi);	//�õ�γ���ַ���
	
 	LCD_DisplayString(50,40,16,dtbuf2);	 	 
	tp=gpsx.altitude;	   
 	sprintf((char *)dtbuf3,"Altitude:%.1fm     ",tp/=10);	    			//�õ��߶��ַ���

 	LCD_DisplayString(50,60,16,dtbuf3);	 			   
//	tp=gpsx.speed;	   
// 	sprintf((char *)dtbuf,"Speed:%.3fkm/h     ",tp/=1000);		    		//�õ��ٶ��ַ���	 
// 	LCD_ShowString(30,180,200,16,16,dtbuf);	 				    
//	if(gpsx.fixmode<=3)														//��λ״̬
//	{  
//		sprintf((char *)dtbuf,"Fix Mode:%s",fixmode_tbl[gpsx.fixmode]);	
//	  LCD_ShowString(30,200,200,16,16,dtbuf);			   
//	}	 	   
//	sprintf((char *)dtbuf,"GPS+BD Valid satellite:%02d",gpsx.posslnum);	 		//���ڶ�λ��GPS������
// 	LCD_ShowString(30,220,200,16,16,dtbuf);	    
//	sprintf((char *)dtbuf,"GPS Visible satellite:%02d",gpsx.svnum%100);	 		//�ɼ�GPS������
// 	LCD_ShowString(30,240,200,16,16,dtbuf);
//	
//	sprintf((char *)dtbuf,"BD Visible satellite:%02d",gpsx.beidou_svnum%100);	 		//�ɼ�����������
// 	LCD_ShowString(30,260,200,16,16,dtbuf);
	
	sprintf((char *)dtbuf,"UTC Date:%04d/%02d/%02d   ",gpsx.utc.year,gpsx.utc.month,gpsx.utc.date);	//��ʾUTC����
	LCD_DisplayString(50,80,16,dtbuf);		    
	sprintf((char *)dtbuf,"UTC Time:%02d:%02d:%02d   ",gpsx.utc.hour,gpsx.utc.min,gpsx.utc.sec);	//��ʾUTCʱ��
  LCD_DisplayString(50,100,16,dtbuf);		  
}

/////////////////
//��ʾATK-HC05ģ�������״̬



