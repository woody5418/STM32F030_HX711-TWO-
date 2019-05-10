//********************************************************************
//
//      代码功能：ESP8266 JSON解析代码
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"


uint8_t   FLAG_CJSON_SEND = 0;     // 定时器发送cJson控制变量

unsigned int strength1_status_value=0;
unsigned int strength2_status_value=0;

unsigned int strength1_check_value=0;
unsigned int strength2_check_value=0;

uint8_t js_strength1_status[10];
uint8_t js_strength2_status[10];

char json_get_temp[200] = "";  //二级Json存储 数组
/**
 * 接收到串口1的ESPP8266数据
 * 判断是否为JSON，并且执行相应的功能
 */
void Apps_Analysis_Data(void)
{
    uint8_t jsFirstOk[3];  //第一次接收的OK

    if (USART1_RX_STA & 0x8000)
    {
		json_get_ItemObjectFirst((char *)USART1_RX_BUF,"welcome",json_get_temp);
		if (json_check(json_get_temp)  == 1) //合法 为0  不合法为1
		{
			json_get_value((char *)json_get_temp,"status",(char *)jsFirstOk);
		}else
		if(checkJSON("Connection: close"))
		{
			TIM_Cmd(TIM2, DISABLE);//数据任务关
			Control_Flag_Mode=0;
			Dis_Number = 10;  //进入Airkiss模式
			//灯效果+
			GPIO_ResetBits(GPIOA, GPIO_Pin_3); //1红灯1
			delay_ms(40);
			GPIO_SetBits(GPIOA, GPIO_Pin_3);//1红灯1
			delay_ms(40);
			GPIO_ResetBits(GPIOA, GPIO_Pin_3); //1红灯1
			delay_ms(40);
			GPIO_SetBits(GPIOA, GPIO_Pin_3);//1红灯1
			delay_ms(40);
		}
		memset(USART1_RX_BUF,0,300);
		USART1_RX_STA=0;
    }
}
void  Cantilever_Read(void)
{
	strength1_status_value = Read_L_HX711()/100;
	strength2_status_value = Read_R_HX711()/100;
}

/**
 * 发送数据到服务器
* 发送的{"id":"SWTZ_Cantilever_a0:20:a6:29:a7:dc","to":"user","services":{"strength1":"0.001","threshold1":"132","threshold2":"123","strength2":"0.002"}}
 * 接收到的
 */
void send_data_user(void)
{
    char PRemove[32];
    uint8_t packingData[256];        //最大800字节
	
	strcpy((char *) packingData, "{\"id\":\"SWTZ_Cantilever_");
    strcat((char *) packingData, macbufs);
    strcat((char *) packingData, "\",\"to\":\"user\",\"services\":{\"strength1\":\"");
		sprintf((char *)PRemove,"%d",strength1_status_value);  //strength1_status_value
    strcat((char *) packingData, (char *)PRemove);  //strength1的值;
	
	strcat((char *) packingData, "\",\"threshold1\":\"");
		sprintf((char *)PRemove,"%d",strength1_check_value);  //strength1_status_value
	strcat((char *) packingData, (char *)PRemove);  //strength1的值;
	
	strcat((char *) packingData, "\",\"strength2\":\"");
		sprintf((char *)PRemove,"%d",strength2_status_value);  //strength2_status_value
    strcat((char *) packingData, (char *)PRemove);  //strength1的值;
	
	strcat((char *) packingData, "\",\"threshold2\":\"");
		sprintf((char *)PRemove,"%d",strength2_check_value);  //strength2_status_value
    strcat((char *) packingData, (char *)PRemove);  //strength1的值;
	
	strcat((char *) packingData, "\"}}");
    sendMessagePacket((char *)packingData);
}


////////////********************** JSON  数据处理 ***************************////////////////

/***************************************
 name:       JSON 校验函数
 input:      字符串
 output:     合法JAON 返回1 不合法JSON 返回0
 description:
 ***************************************/
char json_check(char *str)
{
    uint16_t str_length;//定义字符长度变量
    str_length = strlen(str);//计算字符长度
    if (str[0] == '{' && str[str_length - 1] == '}')  //通过首尾大括号判断是否为JSON
    {
        return 1;//如果字符串为合法JSON 返回1
    }
    else
    {
        return 0;//如果字符串为合法JSON 返回0
    }
}

/***************************************
 name:       JSON 获取键值函数
 input:      JSON字符串 要获取的键名 获取键值的字符串
 output:     如果获取成功返回1 获取失败返回0
 description:
 ***************************************/
char json_get_value(char *json, char *json_key, char *json_value)
{
    char *json_key_start;//定义键名开始的位置
    char *json_key_end;//定义键名结束的位置
    char json_key_length;//定义键名长度
    char *json_value_start;//定义键值开始的位置
    char *json_value_end;//定义键值结束的位置
    char json_value_length;//定义键值长度

    json_key_start = strstr(json, json_key);//获取键名开始的位置
    json_key_length = strlen(json_key);//获取键名的长度
    json_key_end = json_key_start + json_key_length;//获取键名结束的位置
    if (json_key_start != 0 && *(json_key_start - 1) == '\"'
            && *(json_key_end) == '\"' && *(json_key_end + 1) == ':'
            && *(json_key_end + 2) == '\"')
    {
        json_value_start = json_key_end + 3;//获取键值开始的位置
        json_value_end = strstr(json_value_start, "\""); //获取键值结束的位置
        json_value_length = json_value_end - json_value_start;//获取键值的长度
        strncpy(json_value, json_value_start, json_value_length);//将键值存入指针数组
        json_value[json_value_length] = '\0';//指针最后一位补字符串结束符 \0
        return 1;//成功获取键值 返回1
    }
    else
    {
        json_value[0] = '\0';
        return 0;//失败获取键值 返回0
    }
}

/***************************************
 name:      JSON 提取子JSON键值的值（一级提取）
 input:     JSON字符串 要获取的键名 获取键值的值
 output:    无
 description:
 ***************************************/
void json_get_ItemObject(char *json, char *json_key, char *json_value)
{
    int  json_len;
    uint16_t jsoncls_0=0;
    char *json_get_Item_obj;
    char *ptr_obj;

    while(json[jsoncls_0++] != '\0');
    json[jsoncls_0-1] = 0x30;
    json_get_Item_obj = strstr(json, json_key);
    json_get_Item_obj = strstr(json_get_Item_obj, "{");
    ptr_obj = strrchr(json_get_Item_obj, ',');
    json_len = strlen(json_get_Item_obj) - strlen(ptr_obj);
    strncpy(json_value, json_get_Item_obj, json_len);
    json_value[json_len] = '\0';
}

/***************************************
 name:      JSON 提取子JSON键值的值
 input:     JSON字符串 要获取的键名 获取键值的值
 output:    无
 description:
 ***************************************/
void json_get_ItemObjectFirst(char *json, char *json_key, char *json_value)
{
    int  json_len_Fir;
    char *json_get_Item_obj_Fir;
    char *ptr_obj_Fir;

    json_get_Item_obj_Fir = strstr(json, json_key);
    json_get_Item_obj_Fir = strstr(json_get_Item_obj_Fir, "{");
    ptr_obj_Fir = strrchr(json_get_Item_obj_Fir, '}');
    json_len_Fir = strlen(json_get_Item_obj_Fir) - strlen(ptr_obj_Fir);
    strncpy(json_value, json_get_Item_obj_Fir, json_len_Fir);
    json_value[json_len_Fir] = '\0';
}


/***************************************
 name:      JSON 提取子JSON(二级提取)
 input:     JSON字符串 要获取的键名 获取子json的串
 output:    无
 description:
 ***************************************/
void json_get_obj(char *json, char *json_key,char *json_objectItem)
{
    int json_len_t;
    char *json_get_temp_obj_t;
    char *ptr_t;

    json_get_temp_obj_t = strstr(json,json_key);
    json_get_temp_obj_t = strstr(json_get_temp_obj_t,"{");
    ptr_t = strchr(json_get_temp_obj_t,'}');
    json_len_t =strlen(json_get_temp_obj_t)-strlen(ptr_t)+1;
    strncpy(json_objectItem, json_get_temp_obj_t, json_len_t);
    json_objectItem[json_len_t] = '\0';
}


/***************************************
 name:       JSON 键值对比函数
 input:      JSON 键值 要匹配的字符
 output:     如果匹配成功返回1 失败返回0
 description:
 ***************************************/
char json_check_value(char *str1, char *str2)
{
    if (strcmp(str1, str2) == 0)
    {
        return 1;//匹配成功返回1
    }
    else
    {
        return 0;//匹配成功返回0
    }
}


/***************************************
 name:       string类型转int类型
 input:      存储string的参数
 output:     如果转换成功返回数值 失败返回0
 description:
 ***************************************/
uint32_t stringtoint(char *a)
{
    uint32_t val=0;
    uint8_t i=0;
    while(1)
    {
        if( *(a+i) == '\0' )break;
        else
        {
            if(*(a+i)<='9'&&*(a+i)>='0')
                val=(val*10)+(*(a+(i++))&0x0F);
            else
                return 0;
        }
    }
    return val;
}


/***************************************
 name:       检测接收到的里面是否存在某一串字符串
 input:      存储string的参数
 output:     如果转换成功返回数值 失败返回0
 description:
 ***************************************/
char* checkJSON(char *str)
{
    char *strx=0;
    strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
    return (char*)strx;
}

