//********************************************************************
//	 
//      代码功能：LED初始化代码	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"

//PA0 ->DT   左
//PA1 ->CLK
/*--------------*/
//PA6 ->DT   右
//PA7 ->CLK
/*--------------*/

void HX711_L_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*****************************************************************/
void HX711_R_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //开漏
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


unsigned long Read_L_HX711(void)  //读HX711芯片输出的数据。 
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(GPIOA,GPIO_Pin_0);    //DOUT=1 
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);    //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));   //等待DOUT=0  
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(GPIOA,GPIO_Pin_1);    //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);    //SCK=0 
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))   //DOUT=1 
			val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(GPIOA,GPIO_Pin_1); 
	val = val^0x800000;
	delay_us(1); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
	delay_us(1);  
	return val;  
} 

unsigned long Read_R_HX711(void)  //读HX711芯片输出的数据。 
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 

	GPIO_SetBits(GPIOA,GPIO_Pin_6);    //DOUT=1 
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);    //SCK=0 
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));   //等待DOUT=0  
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(GPIOA,GPIO_Pin_7);    //SCK=1 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);    //SCK=0 
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))   //DOUT=1 
			val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(GPIOA,GPIO_Pin_7); 
	val = val^0x800000;
	delay_us(1); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_7); 
	delay_us(1);  
	return val;  
} 

void hx711_Init(void)
{
	HX711_L_Init();
	HX711_R_Init();
}

/*
float hx711_data(unsigned long  data,float checkData)
{
	unsigned long temp = 0;
	float aa;
	
	temp = data/100;
	temp = temp*0.128;
	aa = (unsigned long)((((float)temp/4.30+0.05)-checkData));
	//cc = 8.46666*(float)temp;
	//dd = 0.000998*(float)temp*(float)temp;
	//Weight_Shiwu = cc - dd;
	
	return aa;
}
*/
