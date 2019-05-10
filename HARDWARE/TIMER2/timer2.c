//********************************************************************
//	 
//      ���빦�ܣ���ʱ��2��ʼ������	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


uint8_t FLAG_LED_delay = 0; //����LEDʱ�����ʱ����
uint8_t FLAG_delay_s = 0; //�����ϴ����ݱ���
uint8_t FLAG_Data_read = 0; //�������ݶ�ȡ
static uint8_t delay_times = 0;  // ���ͼ�ʱ��������
static uint8_t data_times = 0;  // ��ȡ���ݼ���
static uint8_t led_flag=0;



// arr = 1000      psr = 7199
void TIM2_Init(uint32_t arr,uint16_t psr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	//     TIM_Period = 1000 ��1S
	TIM_TimeBaseStructure.TIM_Period = arr;  // �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
	TIM_TimeBaseStructure.TIM_Prescaler = psr;	//ʱ��Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);			        // �������жϱ�־ 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		if(FLAG_delay_s == 1) {
            delay_times++;
            FLAG_LED_delay++;
			data_times++;
            if(FLAG_LED_delay == 20) { //500ms
                FLAG_LED_delay=0;
				if(led_flag == 0)
				{
					led_flag=1;
					LEDL;
				}else{
					led_flag=0;
					LEDH;
				}
            }
            if(delay_times == 20) {   //2S
                delay_times = 0;
                FLAG_CJSON_SEND=1;
            }
			if(data_times == 10)  //500ms
			{
				data_times=0;
				FLAG_Data_read = 1;
			}
        }
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);				//����жϱ�־λ
}


