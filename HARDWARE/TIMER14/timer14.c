//********************************************************************
//	 
//      ���빦�ܣ���ʱ��14��ʼ������ ��1s��ʱ��	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


uint8_t NewWeekNum = 0x01;  //��ǰ����
uint32_t NewTimeH = 0;   //��ǰpwm����ֵ
uint32_t NewTimeL = 0;   //��ǰpwm����ֵ

void TIM14_Init(uint32_t arr,uint16_t psr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;  // �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
	TIM_TimeBaseStructure.TIM_Prescaler = psr;	//ʱ��Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM14, TIM_FLAG_Update);			        // �������жϱ�־ 
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM14, ENABLE);
}

void TIM14_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
/********************************     �Զ�����     ********************************/
  /***************************** ����ʱ��ֻ��һ���� *****************************/
		FirstRECVTime++; //����
		if(FirstRECVTime == 86400)  //����24H��ʱ������0
		{
			FirstRECVTime=0;  //���¿�ʼ�µ�һ��
			NewWeekNum = (NewWeekNum<<1);
			if((NewWeekNum &0x80) != 0 )   //��
				NewWeekNum = 0x01;
		}
		if((mode_status_value&0x01)==0)//������AD����
		{
			ADC_CP_status=1; //������ģʽ
		}
		
		if(mode_status_value&0x04)
		{
			if(NewTimeL==0)
				NewTimeH++;
			if(NewTimeH == 0) 
				NewTimeL++;
			
			if(NewTimeH >= repeat_t1_value)
			{
				NewTimeH=0;
				NewTimeL=1;
				if(ADC_CP_status == 1)
					OUTH;  //��һ�������
			}
			if(NewTimeL >= repeat_t2_value)
			{
				NewTimeL=0;
				if(ADC_CP_status == 1)
					OUTL;  //��һ�������
			}
		}else
		{
			NewTimeL=0;
			NewTimeH=0;
		}
	}
	TIM_ClearITPendingBit(TIM14, TIM_FLAG_Update);
}


