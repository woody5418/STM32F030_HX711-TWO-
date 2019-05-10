//********************************************************************
//	 
//      代码功能：按键初始化代码	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"


//PA5   清零按钮
//PA8按键
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* 配置KEY相应引脚PA4*/
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


// 按键PA4 上来输入   按下去3S
void KEY_SCAN(void)
{
	uint8_t KeyNumFlag = 0; //按键计数函数
	
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) { 
        delay_ms(500); // 去抖动
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) {
            KeyNumFlag++;
            if(KeyNumFlag == 6) {  //3s
                KeyNumFlag=0;
				LEDL;
                Control_Flag_Mode = 2;  //进入Airkiss模式
                NetNumber=0;
                LEDL;//1红灯
                delay_ms(230);
                LEDH;//0红灯
                delay_ms(230);
                LEDL; //1红灯
                delay_ms(230);
                LEDH;//0红灯
                delay_ms(230);
            }
        }
    }
    KeyNumFlag=0;
}

// 按键PA5 上来输入   按下去3S
void Cantilever_key(void)
{
	//uint8_t KeyNumFlag = 0; //按键计数函数
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) 
	{ 
        delay_ms(100); // 去抖动
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) 
		{
            //KeyNumFlag++;
            //if(KeyNumFlag == 10) {  //3s.
				for(char j=0;j<10;j++)
				{
					strength1_check_value = Read_L_HX711()/100;
					strength2_check_value = Read_R_HX711()/100;
					LEDL;//1红灯
					delay_ms(500);
					LEDH;//0红灯
					delay_ms(500);
				}
            //}                           
        }
    }
    //KeyNumFlag=0;
}



