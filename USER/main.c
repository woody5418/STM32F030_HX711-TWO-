/****************************************************************************
*       Copyright (C), 北京四维拓智教育科技有限公司
*
*       WIFI模块
*       V0.1    
*       2017-06-27 
*       Woody
*< STM32F030F4P6 >
			FLASH SIZE:16Kbytes
			RAM SIZE:4Kbytes
			Package:TSSOP20 
* 已用
			FLASH:15.616Kbytes
			RAM:  2.808Kbytes
*****************************************************************************/


#include "includes.h"



int main(void)
{
	delay_init();
	TIM3_Init(200,7199);  //计数到100是100ms时间
	TIM2_Init(50,7199);	   //计数到50是50ms时间
	usart1_Init(115200);  //串口1初始化用作于ESP8266接收数据
	LED_Init();   //LED灯初始化
	KeyInit();    //按键初始化
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
                send_data_user();    //发送json到服务器
            }
        }
		if(FLAG_Data_read == 1)
		{
			FLAG_Data_read = 0;
			Cantilever_Read();
		}
		switch(Control_Flag_Mode) 
		{
			case 0: {                    //首次进入
				FLAG_delay_s=0;
				TIM_Cmd(TIM2, DISABLE);  //定时器关闭
				Distribution_Network();  //上电以后进入，判断网络连接以及是否进入Airkiss模式
				break;
			}
			case 1: {                    //数据接收发送
				FLAG_delay_s=1;
				Apps_Analysis_Data();    //进入Airkiss模式以后 数据接收发送处理
				break;
			}
			case 2: {
				FLAG_delay_s=0;
				TIM_Cmd(TIM2, DISABLE);  //定时器关闭
				Configuration_Network(); //按键按下以后进入Airkiss模式，退出连接进入Airkiss
				break;
			}
        }
	}
}


/*****END OF FILE****/












