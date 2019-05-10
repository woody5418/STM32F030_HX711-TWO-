//********************************************************************
//	 
//      ���빦�ܣ�������ʼ������	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************


#include "includes.h"


//PA5   ���㰴ť
//PA8����
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* ����KEY��Ӧ����PA4*/
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


// ����PA4 ��������   ����ȥ3S
void KEY_SCAN(void)
{
	uint8_t KeyNumFlag = 0; //������������
	
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) { 
        delay_ms(500); // ȥ����
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) {
            KeyNumFlag++;
            if(KeyNumFlag == 6) {  //3s
                KeyNumFlag=0;
				LEDL;
                Control_Flag_Mode = 2;  //����Airkissģʽ
                NetNumber=0;
                LEDL;//1���
                delay_ms(230);
                LEDH;//0���
                delay_ms(230);
                LEDL; //1���
                delay_ms(230);
                LEDH;//0���
                delay_ms(230);
            }
        }
    }
    KeyNumFlag=0;
}

// ����PA5 ��������   ����ȥ3S
void Cantilever_key(void)
{
	//uint8_t KeyNumFlag = 0; //������������
    while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) 
	{ 
        delay_ms(100); // ȥ����
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) 
		{
            //KeyNumFlag++;
            //if(KeyNumFlag == 10) {  //3s.
				for(char j=0;j<10;j++)
				{
					strength1_check_value = Read_L_HX711()/100;
					strength2_check_value = Read_R_HX711()/100;
					LEDL;//1���
					delay_ms(500);
					LEDH;//0���
					delay_ms(500);
				}
            //}                           
        }
    }
    //KeyNumFlag=0;
}



