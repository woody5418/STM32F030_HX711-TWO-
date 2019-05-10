//********************************************************************
//	 
//      ���빦�ܣ�ADC��ʼ������	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************
#include "includes.h"


void ADC1_Init(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	ADC_InitTypeDef     ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
		
	ADC_StructInit(&ADC_InitStructure);//��ʼ��ADC�ṹ
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λ����
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ���Ϊ�Ҷ���
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward; //ADC��ɨ�跽��
    ADC_Init(ADC1, &ADC_InitStructure); 
	 
    ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_239_5Cycles);
	
	ADC_GetCalibrationFactor(ADC1);//У׼
    ADC_Cmd(ADC1, ENABLE);         //ʹ��			
}

uint16_t ADC_GetValue(void)
{
	uint16_t adcvalue=0;
	
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADEN) == RESET);
			ADC_StartOfConversion(ADC1); 
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);  //�ȴ�ADC��� 
			adcvalue = ADC_GetConversionValue(ADC1);
	return adcvalue;
}

