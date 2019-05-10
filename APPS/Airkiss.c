//********************************************************************
//
//      ���빦�ܣ�Airkiss��������
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"

uint8_t Dis_Number = 0;   //��ʼ���Լ��������̿��Ʊ���
uint8_t NetNumber = 0;   //����Airkiss�������̱���
uint8_t Control_Flag_Mode=0; // ����ģ��ģʽ�л�


//********************************
//   ��������WIFI ѡ��Airkiss
//********************************
void Distribution_Network(void)
{
    switch(Dis_Number)
    {
    case 0:                          //��ʼ�� ���˳�͸��Ȼ������������
    {
        Send_Order_RST();             //������������ESP8266
        for(uint8_t m=100; m>0; m--) //�ȴ�10s����wifi
        {
            LEDL;
            delay_ms(50);
            LEDH;
            delay_ms(50);
        }
        LEDH;
        Dis_Number = 1;               //�����ɹ�
        break;
    }
    case 1:                                    //����Ƿ���AP����
    {
        if(Send_Search_Connect_AP() == true)
            Dis_Number = 5; //������
        else
            Dis_Number=2; //û������
        break;
    }
    case 2:               	                     //ѡ��stationģʽ
    {
        Send_WiFi_Mode();
        Dis_Number=3; //����Airkissģʽ
        break;
    }
    case 3:                                    //����airkiss
    {
        LEDL;
        if (Send_Start_Airkiss() == true)
        {
            Dis_Number=5; //����TCP
        }
        else
        {
            Dis_Number = 4;//ֹͣairkiss
        }
        break;
    }
    case 4:                                    //ֹͣAirkiss
    {
        if (Send_Stop_Airkiss() == true)
            Dis_Number=3; //����Airkissģʽ
        else
            Dis_Number = 4;//ֹͣairkiss
        break;
    }
    case 5:                                     //����TCP
    {
        LEDH;
        if (Send_Set_Connect_TCP() == true)
            Dis_Number=7; //����TCP����
        else
            Dis_Number = 6;//�ر�TCP
        break;
    }
    case 6:                                     //�ر�TCP
    {
        if (Send_CLOSE_TCP_UDP() == true)
            Dis_Number=5; //��ѯMAC��ַ
        else
            Dis_Number = 3;//�ر�TCP
        break;
    }
    case 7:                                     //��ѯMAC��ַ
    {
        if (Send_Search_Local_MAC() == true)
            Dis_Number=8;
        else
            Dis_Number = 7;
        break;
    }
    case 8:                                     //������
    {
        Send_Passthrough();
        Dis_Number=9;
        break;
    }
    case 9:                                     //����͸��
    {
        Send_Data_Passthrough();
        Dis_Number=10; //���ӳɹ�
        break;
    }
    case 10:                                    //��������ͷ�ɹ�
    {
        Send_Request_header();
        Dis_Number=11;
        TIM_Cmd(TIM2, ENABLE);//��������
        Control_Flag_Mode=1;
        break;
    }
    }
}


//********************************
//   ����Airkissģʽ
//********************************
void Configuration_Network(void)
{
    switch(NetNumber)
    {
    case 0:   // �˳�TCPģʽ
    {
        LEDH;

        if(Quit_Order_TT() ==true)
            NetNumber=1; //�Ͽ�AP
        else
            NetNumber=0;
        break;
    }
    case 1:   //�Ͽ�AP
    {
        Send_CLOSE_TCP_UDP();
        NetNumber=2;  //����Airkiss
    }
    case 2:
    {
        Control_Flag_Mode=0;
        Send_Stop_Airkiss();
        Dis_Number = 3;  //����Airkissģʽ
        break;
    }
    }
}



