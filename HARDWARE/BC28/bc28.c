#include "delay.h"

#include "bc28.h"
#include "string.h"
char *strx,*extstrx;
char atstr[BUFLEN];
char atstr2[BUFLEN];
int err;    //ȫ�ֱ���
char atbuf[BUFLEN];
char objtnum[BUFLEN];//�۲��
char distnum[BUFLEN];//�۲��
BC28 BC28_Status;

#define PRODUCEKEY "a1oXv3YiB3w"        //�޸Ĳ�Ʒ��Կ
#define DEVICENAME "WATER"           //�޸��豸����
#define DEVICESECRET "5f02bad2fb27c3a31703141109c02662" //�޸��豸��Կ
#define SUBSCRIBE "/a1oXv3YiB3w/WATER/user/get"      //�������
//#define ISSUE "/sys/a1oXv3YiB3w/WATER/thing/event/property/post"
#define ISSUE "/a1oXv3YiB3w/WATER/user/update"

void Clear_Buffer(void)//��մ���2����
{
    printf("%s\r\n",buf_uart2.buf);  //���ǰ��ӡ��Ϣ
		delay_ms(300);
    buf_uart2.index=0;
    memset(buf_uart2.buf,0,BUFLEN);
}

int BC28_Init(void)
{
    int errcount = 0;
    err = 0;    //�ж�ģ�鿨�Ƿ��������Ҫ
    printf("start init bc28\r\n");
    Uart2_SendStr("ATE1\r\n");
    delay_ms(300);
    printf("%s\r\n",buf_uart2.buf);      //��ӡ�յ��Ĵ�����Ϣ
    printf("get back bc28\r\n");
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        printf("\r\n��Ƭ���������ӵ�ģ��...\r\n");
        Clear_Buffer();	
        Uart2_SendStr("AT\r\n");
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    }
    Uart2_SendStr("AT+CMEE=1\r\n"); //�������ֵ
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    Clear_Buffer();	

    Uart2_SendStr("AT+NBAND?\r\n"); //�������ֵ
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    if(strx)
    {
        printf("======== BAND========= \r\n %s \r\n",buf_uart2.buf);
        Clear_Buffer();
        delay_ms(300);
    }
    
    Uart2_SendStr("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR");//ֻҪ�������� �����ͳɹ�
    if(strx==NULL)
    {
        printf("�ҵĿ����� : %s \r\n",buf_uart2.buf);
        Clear_Buffer();	
        delay_ms(300);
    }
    else
    {
        err = 1;
        printf("������ : %s \r\n",buf_uart2.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart2_SendStr("AT+CGATT=1\r\n");//�������磬PDP
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//��OK
    Clear_Buffer();	
    if(strx)
    {
        Clear_Buffer();	
        printf("init PDP OK\r\n");
        delay_ms(300);
    }
    Uart2_SendStr("AT+CGATT?\r\n");//��ѯ����״̬
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//��1 ��������ɹ� ��ȡ��IP��ַ��
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();	
        Uart2_SendStr("AT+CGATT?\r\n");//��ȡ����״̬
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//����1,����ע���ɹ�
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }


    Uart2_SendStr("AT+CSQ\r\n");//�鿴��ȡCSQֵ
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSQ");//����CSQ
    if(strx)
    {
        printf("�ź�����:%s\r\n",buf_uart2.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart2_SendStr("AT+CEREG?\r\n");
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:0,1");//����ע��״̬
    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:1,1");//����ע��״̬
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)//��������ֵ��û��
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CEREG?\r\n");//�ж���Ӫ��
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:0,1");//����ע��״̬
        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:1,1");//����ע��״̬
        if(errcount>100)     //��ֹ��ѭ��
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC28_PDPACT(void)//�������Ϊ���ӷ�������׼��
{
    int errcount = 0;
    Uart2_SendStr("AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n");//����APN
    delay_ms(300);
    Uart2_SendStr("AT+CGATT=1\r\n");//�����
    delay_ms(300);
    Uart2_SendStr("AT+CGATT?\r\n");//�����
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)" +CGATT:1");//ע����������Ϣ
    Clear_Buffer();	
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CGATT?\r\n");//�����
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//һ��Ҫ�ն�����
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    Uart2_SendStr("AT+CSCON?\r\n");//�ж�����״̬������1���ǳɹ�
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,1");//ע����������Ϣ
    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,0");//ע����������Ϣ
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)    //���Ӳ��ܷ�������
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CSCON?\r\n");//
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,1");//
        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,0");//
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }

}





/////////////////////////////////////////
void BC28_ConLWM2M(void)
{
    int errcount = 0;
    int i=0;
    Uart2_SendStr("AT+MIPLCREATE\r\n"); //+MIPLCREATE: 0
    delay_ms(500);
    delay_ms(500);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");
    if(strx==NULL)
    {
        printf("����LWM2MЭ��ʧ��\r\n");
        Clear_Buffer();
        Uart2_SendStr("AT+MIPLCREATE\r\n"); //+MIPLCREATE: 0
        delay_ms(500);
        delay_ms(500);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");
        if(strx)
        {
            printf("����LWM2MЭ��ɹ�\r\n");
        }
        else
        {
           strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR: 653");
           if(strx)
           {
               GPIO_SetBits(GPIOC,GPIO_Pin_7);		//ģ������
               delay_ms(500);
               GPIO_ResetBits(GPIOC,GPIO_Pin_7);
               delay_ms(300);
               NVIC_SystemReset();	//û�д���TCP SOCKET������ϵͳ�ȵ�����������
           }
        }
         Clear_Buffer();
    }
    Clear_Buffer();

    Uart2_SendStr("AT+MIPLADDOBJ=0,3303,1,1,1,0\r\n");//�¶�ʵ��
    delay_ms(300);
    Clear_Buffer();
    delay_ms(300);

    Uart2_SendStr("AT+MIPLOPEN=0,86400\r\n");//��
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLOBSERVE");//�����۲��
    memset(atbuf,0,BUFLEN);
    memset(objtnum,0,BUFLEN);
    while(1)
    {
        delay_ms(30);
        if(strlen((const char *)buf_uart2.buf) > 0)
        {
            strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLOBSERVE");//��⵽�۲�Ž���
            if(strx)
            {
                sprintf(atbuf,"%s",buf_uart2.buf);
                strx=strstr((const char*)strx+1,(const char*)",");
                for(i=0;;i++)   //��ѯ�۲��
                {
                    if(strx[i+1]==',')
                    break;
                    objtnum[i]=strx[i+1];
                }
                printf("�õ�AT����:atbuf = %s \r\n",atbuf);
                printf("�õ��۲��:objtnum = %s \r\n",objtnum);
                break;
            }
            else    //���û���ҵ�Ŀ�����շ��������飬�ҵ��˾�������
            {
                printf("���ӷ���:%s\r\n",buf_uart2.buf);
                Clear_Buffer();
            }
        }
    }
    Clear_Buffer();

    printf("object number = %s\r\n",objtnum);
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+MIPLOBSERVERSP=0,%s,1\r\n",objtnum);
    Uart2_SendStr(atstr);//���͹۲�����
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLDISCOVER");//����OK
    memset(atbuf,0,BUFLEN);
    memset(distnum,0,BUFLEN);
    while(1)
    {
        delay_ms(30);
        if(strlen((const char *)buf_uart2.buf) > 0)
        {
            strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLDISCOVER");//��⵽�۲�Ž���
            if(strx)
            {
                sprintf(atbuf,"%s",buf_uart2.buf);
                strx=strstr((const char*)strx+1,(const char*)",");
                for(i=0;;i++)   //��ѯ�۲��
                {
                    if(strx[i+1]==',')
                    break;
                    distnum[i]=strx[i+1];
                }
                printf("�õ�AT����:atbuf = %s \r\n",atbuf);
                printf("�õ���Դ��:distnum = %s \r\n",distnum);
                break;
            }
            else    //���û���ҵ�Ŀ�����շ��������飬�ҵ��˾�������
            {
                Clear_Buffer();
            }
        }
    }
    Clear_Buffer();

    printf("discover number = %s\r\n",distnum);
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+MIPLDISCOVERRSP=0,%s,1,4,\"5700\"\r\n",distnum);
    Uart2_SendStr(atstr);//������Դ����
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    Clear_Buffer();

}
void BC28_Senddata(uint8_t *len,uint8_t *data)
{
    int errcount=0;
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+MIPLNOTIFY=0,%s,3303,0,5700,4,%s,%s,0,0\r\n",objtnum,len,data);
    Uart2_SendStr(atstr);//����0 socketIP�Ͷ˿ں������Ӧ���ݳ����Լ�����
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//����OK
        if(errcount>100)     //��ֹ��ѭ��
        {
            errcount = 0;
            break;
        }
    }
    Clear_Buffer();	
}





void BC28_RegALIYUNIOT(void)//ƽ̨ע��
{
    u8  BC20_IMEI[20],i;//IMEIֵ
		int errcount = 0;
    Uart2_SendStr("AT+QMTDISC=0\r\n");//Disconnect a client from MQTT server
    delay_ms(300);
    Clear_Buffer();

    Uart2_SendStr("AT+QMTCLOSE=0\r\n");//ɾ�����
    delay_ms(300);
    Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//���Ͱ��������ò���
    delay_ms(300);  //�ȴ�300ms����OK
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//��OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//��OK
    }
    Clear_Buffer();

    Uart2_SendStr("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n");//��¼������ƽ̨
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTOPEN: 0,0");//��+QMTOPEN: 0,0
    while(strx==NULL)
    {
				errcount++;
				delay_ms(500);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTOPEN: 0,0");//����OK
        if(errcount>100)     //��ֹ��ѭ��
        {
             GPIO_SetBits(GPIOC,GPIO_Pin_7);		//ģ������
             delay_ms(500);
             GPIO_ResetBits(GPIOC,GPIO_Pin_7);
             delay_ms(300);
             NVIC_SystemReset();	//û�д���TCP SOCKET������ϵͳ�ȵ�����������
        }
    }
    Clear_Buffer();

    Uart2_SendStr("AT+CGSN=1\r\n");//��ȡģ���IMEI��
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGSN:");//��+CGSN:
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGSN:");//��+CGSN:
    }
    for(i=0;i<15;i++)
        BC20_IMEI[i]=strx[i+7];
    BC20_IMEI[15]=0;
    Clear_Buffer();

    printf("�ҵ�ģ��IMEI��:%s\r\n",BC20_IMEI);

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//�������ӵ�������
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTCONN: 0,0,0");//��+QMTCONN: 0,0,0
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTCONN: 0,0,0");//��+QMTCONN: 0,0,0
    }
    Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//�������ӵ�������
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTSUB: 0,1,0,1");//��OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTSUB: 0,1,0,1");//��OK
    }
    Clear_Buffer();
}

//void BC28_ALYIOTSenddata(u8 *len,u8 *data)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
//{
//    memset(atstr,0,BUFLEN);
//    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"%s\",\"{params:{PH:%s}}\"\r\n",ISSUE,data);
//    printf("atstr = %s \r\n",atstr);
//    Uart2_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
//    delay_ms(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
//    while(strx==NULL)
//    {
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,1,0");//��SEND OK
//    }
//    Clear_Buffer();
//}

void BC28_ALYIOTSenddata1(u8 *len,u8 *data)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{
    memset(atstr,0,BUFLEN);
	  memset(atstr2,0,BUFLEN);
		sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);

    Uart2_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
		delay_ms(200);
		sprintf(atstr2,"{params:{\"PH\":%s}}\r\n",data);
		Uart2_SendStr(atstr2);
		delay_ms(200);
		USART_SendData(USART2,0x1A);
//    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//��SEND OK
    }
    Clear_Buffer();
}

void BC28_ALYIOTSenddata2(u8 *len,u8 *data,u8 *ZF)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{
    memset(atstr,0,BUFLEN);
	  memset(atstr2,0,BUFLEN);
		sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);

    Uart2_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
		delay_ms(200);
		sprintf(atstr2,"{params:{\"%s\":%s}}\r\n",ZF,data);
		Uart2_SendStr(atstr2);
		delay_ms(200);
		USART_SendData(USART2,0x1A);
//    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//��SEND OK
    }
    Clear_Buffer();
}
void BC28_ALYIOTSenddata3(u8 *len,u8 *data1,u8 *data2,u8 *data3)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{
    memset(atstr,0,BUFLEN);
	  memset(atstr2,0,BUFLEN);
		sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);

    Uart2_SendStr(atstr);//����IMEI,LWM2MЭ��Ҫ������IMEI��Ϣ
		delay_ms(200);
		sprintf(atstr2,"{params:{\"CoordinateSystem\":1,\"Latitude\":%s,\"Longitude\":%s,\"Altitude\":%s}\r\n",data1,data2,data3);
		Uart2_SendStr(atstr2);
		delay_ms(200);
		USART_SendData(USART2,0x1A);
//    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//��SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//��SEND OK
    }
    Clear_Buffer();
}
