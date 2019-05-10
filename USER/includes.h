/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM32F030 Evaluation Board
*
* Filename      : includes.h
* Version       : V1.10
* Programmer(s) : BAN
*************************************************************************************************/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stm32f0xx.h"



extern char  *macbufs;             //mac 地址控制

extern uint8_t Dis_Number;         //初始化以及配网流程控制变量
extern uint8_t NetNumber;          //进入Airkiss控制流程变量
extern uint8_t Control_Flag_Mode;  // 控制模块模式切换
extern uint8_t FLAG_CJSON_SEND;    // 定时器发送cJson控制变量
extern uint8_t FLAG_delay_s;       //控制上传数据变量
extern uint8_t FLAG_Data_read;    //控制数据读取
extern unsigned int strength1_check_value;
extern unsigned int strength2_check_value;
/*
 *常用头文件
 */
#include "delay.h"

/*
 *wifi部分头文件
 */
#include "websocket.h"
#include "esp8266.h"
#include "espjson.h"
#include "Airkiss.h"

/*
 *驱动库头文件
 */
#include "USART1.h"
#include "timer2.h"
#include "timer3.h"
//#include "timer14.h"
#include "led.h"
#include "key.h"
#include "hx711.h"
/*
 *C语言库头文件
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "stdbool.h"


#endif
/**********************************     END     **********************************/









