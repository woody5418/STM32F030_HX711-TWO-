/********************************************************************
 *@file        usart1.h
 *@author      Woody      QQ��2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief       
 *******************************************************************/
#ifndef __USART1_H
#define __USART1_H

#include "stm32f0xx.h"
#include <stdio.h>

#define USART1_REC_LEN  	256
#define USART1_SEN_LEN  	300


extern uint16_t USART1_RX_STA;       //����״̬���
extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];   //�������飬������64�ֽ�


void usart1_Init(uint32_t baud);
void u1_printf(char* fmt,...);
#endif







