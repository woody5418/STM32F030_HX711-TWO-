//********************************************************************
//
//      ���빦�ܣ�ESP8266���������
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"


char *connStr = "AT+CIPSTART=\"TCP\",\"4dclass.net\",80"; //TCP���ӷ����� �˿�80
/************************����ͷ������**************************/

/************************Ŀǰ����BASE64����********************/
char *projectname="SWTZ_Cantilever_"; //ͨѶID����ʶ
char *info1 = "GET http://4dclass.net/WebSocket/chat/"; //ͨѶURL
char *info2 = " HTTP/1.1\r\nHost: -.net\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\r\nOrigin: http://4dclass.net\r\nSec-WebSocket-Protocol: chat, superchat\r\nSec-WebSocket-Version: 13\r\nUser-Agent: ESP8266\r\n";
/************************����ͷ������**************************/
char macbuf[18];  //MAC��ַ�洢��ַ
char *macbufs="FFFFFFFFFFFFF";  //MAC��ַָ��
/*************************************************************/

/**
 * ���ô���1��8266��������
 */
void sendCMD(char *cmd)
{
    u1_printf("%s\r\n",cmd);
}

/***********************************************************************/

/**
 * ����8266��Ҳ���ϵ��Լ�ʹ��
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Order_RST(void)
{
    bool state = true;
    u1_printf("+++");
    delay_ms(500);
    delay_ms(500);
    u1_printf("\r\n");
    delay_ms(10);
    state = esp8266SendCMD("AT+RST","OK",20);
    return state;
}
/**
 * �˳�͸��
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Quit_Order_TT(void)
{
    bool state = true;
    u1_printf("+++");
    delay_ms(500);
    delay_ms(500);
    u1_printf("\r\n");
    delay_ms(10);
    state = esp8266SendCMD("AT","OK",20);
    return state;
}
/**
 * ��ȡ8266����״̬���Ƿ���AP����
 * �з���true��û�з���false
 */
bool Send_Search_Connect_AP(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPSTATUS","STATUS:2",50);
    return state;
}

/**
 * ѡ������ģʽ
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_WiFi_Mode(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CWMODE_DEF=1","OK",20);
    return state;
}

/**
 * ѡ��Airkissģʽ,�ȴ���������
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Start_Airkiss(void)
{
    bool state = true;
    //state = esp8266SendCMD("AT+CWJAP_DEF=\"imaker\",\"Wlgf@001\"","OK",1000); //�ȴ��ʱ��10S
    state = esp8266SendCMD("AT+CWSTARTSMART=2","smartconfig connected wifi",30000); //�ȴ��ʱ��5min
    return state;
}

/**
 * ֹͣAirkissѡ������ģʽ
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Stop_Airkiss(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CWSTOPSMART","OK",20);
    return state;
}

/**
 * ����TCP����
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Set_Connect_TCP(void)
{
    bool state = true;
    state = esp8266SendCMD(connStr,"CONNECT",200);
    return state;
}

/**
 * �Ͽ�AP���ӣ��Ͽ�TCP����
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_CLOSE_TCP_UDP(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPCLOSE","OK",20);
    return state;
}

/**
 * ѡ��͸��ģʽ
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Passthrough(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPMODE=1","OK",20);
    return state;
}
/**
 * ���뵽͸��
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Data_Passthrough(void)
{
    bool state = true;
    state = esp8266SendCMD("AT+CIPSEND","OK",20);
    return state;
}

/**
 * ��������ͷ �ȴ��ظ�OK
 */
bool Send_Request_header(void)
{
    bool state = true;
    char handShake[336];  //ƴ������ͷ�Ŀռ�
    //����ͷ info1
    strcpy(handShake,info1);
    strcat(handShake,projectname);
    strcat(handShake,(char *)macbuf);
    strcat(handShake,info2);
    state = esp8266SendCMD(handShake,"OK",20);
    return state;
}

/**
 * ��¼MAC��ַ
 * �ɹ�����true��ʧ�ܷ���false
 */
bool Send_Search_Local_MAC(void)
{
    bool state = true;
    macbufs = Get_Search_Local_MAC("AT+CIFSR",50);
    if(macbufs) state=true;
    else state = false;
    return state;
}

/**
 * ��ATK-ESP8266��������
 * cmd:���͵������ַ���
 * ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
 * waittime:�ȴ�ʱ��(��λ:10ms)
 * ����ֵ:true,���ͳɹ�(�õ����ڴ���Ӧ����)
 *        false,����ʧ��
 */
bool esp8266SendCMD(char *cmd,char *ack,uint16_t waittime)
{
    uint8_t res=true;

    USART1_RX_STA=0;
    sendCMD(cmd);							//��������
    if(ack&&waittime)  				//��Ҫ�ȴ�Ӧ��
    {
        while(--waittime)  		//�ȴ�����ʱ
        {
            delay_ms(10);
            if(USART1_RX_STA&0X8000)  		//���յ��ڴ���Ӧ����
            {
                if(checkCMD((char *)ack))
                {
                    break;		//�õ���Ч����
                }
                USART1_RX_STA=0;
            }
        }
        if(waittime==0)
        {
            res=false;
        }
    }
    return res;
}
/**
 * ATK-ESP8266���������,�����յ���Ӧ��
 * str:�ڴ���Ӧ����
 * ����ֵ:0,û�еõ��ڴ���Ӧ����
 *     ����,�ڴ�Ӧ������λ��(str��λ��)
 */
char* checkCMD(char *str)
{
    char *strx=0;
    if(USART1_RX_STA&0X8000)   		//���յ�һ��������
    {
        USART1_RX_BUF[USART1_RX_STA&0X7FFF]=0;		//��ӽ�����
        strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
    }
    return (char*)strx;
}
/**
 * ��ATK-ESP8266��������
 * cmd:���͵������ַ���
 * waittime:�ȴ�ʱ��(��λ:10ms)
 * ����ֵ:mac��ַ������˫����
 */
char* Get_Search_Local_MAC(char *cmd,uint16_t waittime)
{
    uint8_t n=0;
    char *mac;

    USART1_RX_STA=0;
    sendCMD(cmd);		//��������
    if("STAMAC"&&waittime)  		//��Ҫ�ȴ�Ӧ��
    {
        while(--waittime)  			//�ȴ�����ʱ
        {
            delay_ms(10);
            if(USART1_RX_STA&0X8000)  		//���յ��ڴ���Ӧ����
            {
                if(checkCMD("STAMAC"))
                {
                    mac =  strtok(checkCMD("STAMAC"),",");
                    mac =  strtok(NULL,",");
                    mac =  strtok(mac,"\r\n");
                    for(n=0; n<17; n++)
                    {
                        macbuf[n] = mac[n+1];
                    }
                    macbuf[17] = '\0';
                }
                USART1_RX_STA=0;
            }
        }
        if(waittime==0)
        {
        }
    }
    return (char *)macbuf;
}





