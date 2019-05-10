/********************************************************************
 *@file        led.h
 *@author      Woody      QQ：2490006131
 *@version     V1.0
 *@date        2017-07-26
 *@brief       
 *******************************************************************/
#ifndef __HX711_H
#define	__HX711_H

#include "stm32f0xx.h"

void hx711_Init(void); 
float hx711_data(unsigned long  data,float checkData);
unsigned long Read_L_HX711(void);  //读HX711芯片输出的数据。 
unsigned long Read_R_HX711(void);  //读HX711芯片输出的数据。 
#endif






