//********************************************************************
//	 
//      ���빦�ܣ�����1��ʼ������ ʹ�ö�ʱ��3����ʱ�ж�	   
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"

uint16_t USART1_RX_STA = 0;       //����״̬���
uint8_t  USART1_RX_BUF[USART1_REC_LEN];     //�������� ������64�ֽ�
__align(8) uint8_t USART1_TX_BUF[USART1_SEN_LEN];//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

/* 
 * USART��ʼ�� 
 */
void usart1_Init(uint32_t baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //ʹ��GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  //ʹ��USART��ʱ��
	/* USART1�Ķ˿����� */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //����PA9�ɵڶ���������	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); //����PA10�ɵڶ���������  RX

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART1�Ļ������� */
	USART_InitStructure.USART_BaudRate = baud;              //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
					USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);           //ʹ�ܽ����ж�
	USART_Cmd(USART1, ENABLE);                             //ʹ��USART1

	/* USART1��NVIC�ж����� */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}
//=============================================================================
//�ļ����ƣ�
//���ܸ�Ҫ��USART1�жϺ�����ʹ���ж�3
//����˵������
//�������أ���
//=============================================================================
void USART1_IRQHandler(void)
{
	uint8_t res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		res =USART_ReceiveData(USART1);
        if((USART1_RX_STA&(1<<15))==0) {		//�������һ������,��û�б�����,���ٽ�����������
            if(USART1_RX_STA<USART1_REC_LEN) {		//�����Խ�������
                TIM_SetCounter(TIM3,0);			//���������
                if(USART1_RX_STA==0)
                    TIM_Cmd(TIM3, ENABLE);  	//ʹ�ܶ�ʱ��3
                USART1_RX_BUF[USART1_RX_STA++]=res;	//��¼���յ���ֵ
            }else {
                USART1_RX_STA|=1<<15;		//ǿ�Ʊ�ǽ������
            }
        }
	}
}
/*
 * ����1,printf ����
 * ȷ��һ�η������ݲ����� USART1_SEN_LEN �ֽ�
 */
void u1_printf(char* fmt,...)
{
    uint16_t i,j;
    va_list ap;
    va_start(ap,fmt);
    vsprintf((char*)USART1_TX_BUF,fmt,ap);
    va_end(ap);
    i=strlen((const char*)USART1_TX_BUF);  //�˴η������ݵĳ���
    for(j=0; j<i; j++) {																																		//ѭ����������
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);	//�ȴ��ϴδ������
        USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]);	//�������ݵ�����3
    }
}






