#include "delay.h"

#include "bc28.h"
#include "string.h"
char *strx,*extstrx;
char atstr[BUFLEN];
char atstr2[BUFLEN];
int err;    //全局变量
char atbuf[BUFLEN];
char objtnum[BUFLEN];//观察号
char distnum[BUFLEN];//观察号
BC28 BC28_Status;

#define PRODUCEKEY "a1oXv3YiB3w"        //修改产品秘钥
#define DEVICENAME "WATER"           //修改设备名称
#define DEVICESECRET "5f02bad2fb27c3a31703141109c02662" //修改设备秘钥
#define SUBSCRIBE "/a1oXv3YiB3w/WATER/user/get"      //订阅入口
//#define ISSUE "/sys/a1oXv3YiB3w/WATER/thing/event/property/post"
#define ISSUE "/a1oXv3YiB3w/WATER/user/update"

void Clear_Buffer(void)//清空串口2缓存
{
    printf("%s\r\n",buf_uart2.buf);  //清空前打印信息
		delay_ms(300);
    buf_uart2.index=0;
    memset(buf_uart2.buf,0,BUFLEN);
}

int BC28_Init(void)
{
    int errcount = 0;
    err = 0;    //判断模块卡是否就绪最重要
    printf("start init bc28\r\n");
    Uart2_SendStr("ATE1\r\n");
    delay_ms(300);
    printf("%s\r\n",buf_uart2.buf);      //打印收到的串口信息
    printf("get back bc28\r\n");
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        printf("\r\n单片机正在连接到模块...\r\n");
        Clear_Buffer();	
        Uart2_SendStr("AT\r\n");
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    }
    Uart2_SendStr("AT+CMEE=1\r\n"); //允许错误值
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    Clear_Buffer();	

    Uart2_SendStr("AT+NBAND?\r\n"); //允许错误值
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    if(strx)
    {
        printf("======== BAND========= \r\n %s \r\n",buf_uart2.buf);
        Clear_Buffer();
        delay_ms(300);
    }
    
    Uart2_SendStr("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR");//只要卡不错误 基本就成功
    if(strx==NULL)
    {
        printf("我的卡号是 : %s \r\n",buf_uart2.buf);
        Clear_Buffer();	
        delay_ms(300);
    }
    else
    {
        err = 1;
        printf("卡错误 : %s \r\n",buf_uart2.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart2_SendStr("AT+CGATT=1\r\n");//激活网络，PDP
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返OK
    Clear_Buffer();	
    if(strx)
    {
        Clear_Buffer();	
        printf("init PDP OK\r\n");
        delay_ms(300);
    }
    Uart2_SendStr("AT+CGATT?\r\n");//查询激活状态
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//返1 表明激活成功 获取到IP地址了
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();	
        Uart2_SendStr("AT+CGATT?\r\n");//获取激活状态
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//返回1,表明注网成功
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }


    Uart2_SendStr("AT+CSQ\r\n");//查看获取CSQ值
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSQ");//返回CSQ
    if(strx)
    {
        printf("信号质量:%s\r\n",buf_uart2.buf);
        Clear_Buffer();
        delay_ms(300);
    }

    Uart2_SendStr("AT+CEREG?\r\n");
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:0,1");//返回注册状态
    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:1,1");//返回注册状态
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)//两个返回值都没有
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CEREG?\r\n");//判断运营商
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:0,1");//返回注册状态
        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CEREG:1,1");//返回注册状态
        if(errcount>100)     //防止死循环
        {
            err=1;
            errcount = 0;
            break;
        }
    }
    return err;
}

void BC28_PDPACT(void)//激活场景，为连接服务器做准备
{
    int errcount = 0;
    Uart2_SendStr("AT+CGDCONT=1,\042IP\042,\042HUAWEI.COM\042\r\n");//设置APN
    delay_ms(300);
    Uart2_SendStr("AT+CGATT=1\r\n");//激活场景
    delay_ms(300);
    Uart2_SendStr("AT+CGATT?\r\n");//激活场景
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)" +CGATT:1");//注册上网络信息
    Clear_Buffer();	
    while(strx==NULL)
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CGATT?\r\n");//激活场景
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGATT:1");//一定要终端入网
        if(errcount>100)     //防止死循环
        {
            errcount = 0;
            break;
        }
    }
    Uart2_SendStr("AT+CSCON?\r\n");//判断连接状态，返回1就是成功
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,1");//注册上网络信息
    extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,0");//注册上网络信息
    Clear_Buffer();	
    errcount = 0;
    while(strx==NULL&&extstrx==NULL)    //连接不能反馈错误
    {
        errcount++;
        Clear_Buffer();
        Uart2_SendStr("AT+CSCON?\r\n");//
        delay_ms(300);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,1");//
        extstrx=strstr((const char*)buf_uart2.buf,(const char*)"+CSCON:0,0");//
        if(errcount>100)     //防止死循环
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
        printf("启动LWM2M协议失败\r\n");
        Clear_Buffer();
        Uart2_SendStr("AT+MIPLCREATE\r\n"); //+MIPLCREATE: 0
        delay_ms(500);
        delay_ms(500);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");
        if(strx)
        {
            printf("启动LWM2M协议成功\r\n");
        }
        else
        {
           strx=strstr((const char*)buf_uart2.buf,(const char*)"ERROR: 653");
           if(strx)
           {
               GPIO_SetBits(GPIOC,GPIO_Pin_7);		//模块重启
               delay_ms(500);
               GPIO_ResetBits(GPIOC,GPIO_Pin_7);
               delay_ms(300);
               NVIC_SystemReset();	//没有创建TCP SOCKET就重启系统等到服务器就绪
           }
        }
         Clear_Buffer();
    }
    Clear_Buffer();

    Uart2_SendStr("AT+MIPLADDOBJ=0,3303,1,1,1,0\r\n");//温度实例
    delay_ms(300);
    Clear_Buffer();
    delay_ms(300);

    Uart2_SendStr("AT+MIPLOPEN=0,86400\r\n");//打开
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLOBSERVE");//反馈观察号
    memset(atbuf,0,BUFLEN);
    memset(objtnum,0,BUFLEN);
    while(1)
    {
        delay_ms(30);
        if(strlen((const char *)buf_uart2.buf) > 0)
        {
            strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLOBSERVE");//检测到观察号结束
            if(strx)
            {
                sprintf(atbuf,"%s",buf_uart2.buf);
                strx=strstr((const char*)strx+1,(const char*)",");
                for(i=0;;i++)   //查询观察号
                {
                    if(strx[i+1]==',')
                    break;
                    objtnum[i]=strx[i+1];
                }
                printf("得到AT命令:atbuf = %s \r\n",atbuf);
                printf("得到观察号:objtnum = %s \r\n",objtnum);
                break;
            }
            else    //如果没有找到目标就清空反馈的数组，找到了就跳出来
            {
                printf("连接反馈:%s\r\n",buf_uart2.buf);
                Clear_Buffer();
            }
        }
    }
    Clear_Buffer();

    printf("object number = %s\r\n",objtnum);
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+MIPLOBSERVERSP=0,%s,1\r\n",objtnum);
    Uart2_SendStr(atstr);//发送观察命令
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLDISCOVER");//返回OK
    memset(atbuf,0,BUFLEN);
    memset(distnum,0,BUFLEN);
    while(1)
    {
        delay_ms(30);
        if(strlen((const char *)buf_uart2.buf) > 0)
        {
            strx=strstr((const char*)buf_uart2.buf,(const char*)"+MIPLDISCOVER");//检测到观察号结束
            if(strx)
            {
                sprintf(atbuf,"%s",buf_uart2.buf);
                strx=strstr((const char*)strx+1,(const char*)",");
                for(i=0;;i++)   //查询观察号
                {
                    if(strx[i+1]==',')
                    break;
                    distnum[i]=strx[i+1];
                }
                printf("得到AT命令:atbuf = %s \r\n",atbuf);
                printf("得到资源号:distnum = %s \r\n",distnum);
                break;
            }
            else    //如果没有找到目标就清空反馈的数组，找到了就跳出来
            {
                Clear_Buffer();
            }
        }
    }
    Clear_Buffer();

    printf("discover number = %s\r\n",distnum);
    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+MIPLDISCOVERRSP=0,%s,1,4,\"5700\"\r\n",distnum);
    Uart2_SendStr(atstr);//发送资源命令
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
        if(errcount>100)     //防止死循环
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
    Uart2_SendStr(atstr);//发送0 socketIP和端口后面跟对应数据长度以及数据
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
    while(strx==NULL)
    {
        errcount++;
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返回OK
        if(errcount>100)     //防止死循环
        {
            errcount = 0;
            break;
        }
    }
    Clear_Buffer();	
}





void BC28_RegALIYUNIOT(void)//平台注册
{
    u8  BC20_IMEI[20],i;//IMEI值
		int errcount = 0;
    Uart2_SendStr("AT+QMTDISC=0\r\n");//Disconnect a client from MQTT server
    delay_ms(300);
    Clear_Buffer();

    Uart2_SendStr("AT+QMTCLOSE=0\r\n");//删除句柄
    delay_ms(300);
    Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCFG=\"ALIAUTH\",0,\"%s\",\"%s\",\"%s\"\r\n",PRODUCEKEY,DEVICENAME,DEVICESECRET);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//发送阿里云配置参数
    delay_ms(300);  //等待300ms反馈OK
    strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"OK");//返OK
    }
    Clear_Buffer();

    Uart2_SendStr("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n");//登录阿里云平台
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTOPEN: 0,0");//返+QMTOPEN: 0,0
    while(strx==NULL)
    {
				errcount++;
				delay_ms(500);
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTOPEN: 0,0");//返回OK
        if(errcount>100)     //防止死循环
        {
             GPIO_SetBits(GPIOC,GPIO_Pin_7);		//模块重启
             delay_ms(500);
             GPIO_ResetBits(GPIOC,GPIO_Pin_7);
             delay_ms(300);
             NVIC_SystemReset();	//没有创建TCP SOCKET就重启系统等到服务器就绪
        }
    }
    Clear_Buffer();

    Uart2_SendStr("AT+CGSN=1\r\n");//获取模块的IMEI号
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGSN:");//返+CGSN:
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+CGSN:");//返+CGSN:
    }
    for(i=0;i<15;i++)
        BC20_IMEI[i]=strx[i+7];
    BC20_IMEI[15]=0;
    Clear_Buffer();

    printf("我的模块IMEI是:%s\r\n",BC20_IMEI);

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTCONN=0,\"%s\"\r\n",DEVICENAME);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//发送链接到阿里云
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTCONN: 0,0,0");//返+QMTCONN: 0,0,0
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTCONN: 0,0,0");//返+QMTCONN: 0,0,0
    }
    Clear_Buffer();

    memset(atstr,0,BUFLEN);
    sprintf(atstr,"AT+QMTSUB=0,1,\"%s\",0 \r\n",SUBSCRIBE);
    printf("atstr = %s \r\n",atstr);
    Uart2_SendStr(atstr);//发送链接到阿里云
    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTSUB: 0,1,0,1");//返OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTSUB: 0,1,0,1");//返OK
    }
    Clear_Buffer();
}

//void BC28_ALYIOTSenddata(u8 *len,u8 *data)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
//{
//    memset(atstr,0,BUFLEN);
//    sprintf(atstr,"AT+QMTPUB=0,1,1,0,\"%s\",\"{params:{PH:%s}}\"\r\n",ISSUE,data);
//    printf("atstr = %s \r\n",atstr);
//    Uart2_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
//    delay_ms(300);
//    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
//    while(strx==NULL)
//    {
//        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,1,0");//返SEND OK
//    }
//    Clear_Buffer();
//}

void BC28_ALYIOTSenddata1(u8 *len,u8 *data)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{
    memset(atstr,0,BUFLEN);
	  memset(atstr2,0,BUFLEN);
		sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);

    Uart2_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
		delay_ms(200);
		sprintf(atstr2,"{params:{\"PH\":%s}}\r\n",data);
		Uart2_SendStr(atstr2);
		delay_ms(200);
		USART_SendData(USART2,0x1A);
//    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//返SEND OK
    }
    Clear_Buffer();
}

void BC28_ALYIOTSenddata2(u8 *len,u8 *data,u8 *ZF)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{
    memset(atstr,0,BUFLEN);
	  memset(atstr2,0,BUFLEN);
		sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);

    Uart2_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
		delay_ms(200);
		sprintf(atstr2,"{params:{\"%s\":%s}}\r\n",ZF,data);
		Uart2_SendStr(atstr2);
		delay_ms(200);
		USART_SendData(USART2,0x1A);
//    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//返SEND OK
    }
    Clear_Buffer();
}
void BC28_ALYIOTSenddata3(u8 *len,u8 *data1,u8 *data2,u8 *data3)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{
    memset(atstr,0,BUFLEN);
	  memset(atstr2,0,BUFLEN);
		sprintf(atstr,"AT+QMTPUB=0,0,0,0,\"%s\"\r\n",ISSUE);

    Uart2_SendStr(atstr);//输入IMEI,LWM2M协议要求填入IMEI信息
		delay_ms(200);
		sprintf(atstr2,"{params:{\"CoordinateSystem\":1,\"Latitude\":%s,\"Longitude\":%s,\"Altitude\":%s}\r\n",data1,data2,data3);
		Uart2_SendStr(atstr2);
		delay_ms(200);
		USART_SendData(USART2,0x1A);
//    delay_ms(300);
    strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//返SEND OK
    while(strx==NULL)
    {
        strx=strstr((const char*)buf_uart2.buf,(const char*)"+QMTPUB: 0,0,0");//返SEND OK
    }
    Clear_Buffer();
}
