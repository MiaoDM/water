#include "usart.h"
#include "stm32f4xx.h" 
void Clear_Buffer(void);//清空缓存	
int BC28_Init(void);
void BC28_PDPACT(void);

void BC28_ConLWM2M(void);
void BC28_RECData(void);
void BC28_Senddata(uint8_t *len,uint8_t *data);
void BC28_RegALIYUNIOT(void);
void BC28_ALYIOTSenddata(u8 *len,u8 *data);
void BC28_ALYIOTSenddata1(u8 *len,u8 *data);
void BC28_ALYIOTSenddata2(u8 *len,u8 *data,u8 *ZF);
void BC28_ALYIOTSenddata3(u8 *len,u8 *data1,u8 *data2,u8 *data3);
typedef struct
{
   uint8_t CSQ;    
   uint8_t Socketnum;   //编号
   uint8_t reclen[10];   //获取到数据的长度
   uint8_t res;      
   uint8_t recdatalen[10];
   uint8_t recdata[100];
} BC28;

