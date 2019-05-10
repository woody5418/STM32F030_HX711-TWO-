/****************************************************************************
*       Copyright (C), ������ά���ǽ����Ƽ����޹�˾
*
*       WIFIģ��
*       V0.1    
*       2017-06-27 
*       Woody
*< STM32F030F4P6 >
			FLASH SIZE:16Kbytes
			RAM SIZE:4Kbytes
			Package:TSSOP20 
* ����
			FLASH:15.616Kbytes
			RAM:  2.808Kbytes
*****************************************************************************/


#include "includes.h"



int main(void)
{
	delay_init();
	TIM3_Init(200,7199);  //������100��100msʱ��
	TIM2_Init(50,7199);	   //������50��50msʱ��
	usart1_Init(115200);  //����1��ʼ��������ESP8266��������
	LED_Init();   //LED�Ƴ�ʼ��
	KeyInit();    //������ʼ��
	hx711_Init();
	while (1)
	{
		KEY_SCAN();
		Cantilever_key();
		if(FLAG_delay_s == 1) 
		{
            if(FLAG_CJSON_SEND == 1) 
			{
                FLAG_CJSON_SEND=0;
                send_data_user();    //����json��������
            }
        }
		if(FLAG_Data_read == 1)
		{
			FLAG_Data_read = 0;
			Cantilever_Read();
		}
		switch(Control_Flag_Mode) 
		{
			case 0: {                    //�״ν���
				FLAG_delay_s=0;
				TIM_Cmd(TIM2, DISABLE);  //��ʱ���ر�
				Distribution_Network();  //�ϵ��Ժ���룬�ж����������Լ��Ƿ����Airkissģʽ
				break;
			}
			case 1: {                    //���ݽ��շ���
				FLAG_delay_s=1;
				Apps_Analysis_Data();    //����Airkissģʽ�Ժ� ���ݽ��շ��ʹ���
				break;
			}
			case 2: {
				FLAG_delay_s=0;
				TIM_Cmd(TIM2, DISABLE);  //��ʱ���ر�
				Configuration_Network(); //���������Ժ����Airkissģʽ���˳����ӽ���Airkiss
				break;
			}
        }
	}
}


/*****END OF FILE****/












