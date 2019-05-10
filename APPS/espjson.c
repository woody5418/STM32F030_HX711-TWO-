//********************************************************************
//
//      ���빦�ܣ�ESP8266 JSON��������
//      4DClass
//      Copyright(C)  2017
//      All rights reserved
//***********************************************************************

#include "includes.h"


uint8_t   FLAG_CJSON_SEND = 0;     // ��ʱ������cJson���Ʊ���

unsigned int strength1_status_value=0;
unsigned int strength2_status_value=0;

unsigned int strength1_check_value=0;
unsigned int strength2_check_value=0;

uint8_t js_strength1_status[10];
uint8_t js_strength2_status[10];

char json_get_temp[200] = "";  //����Json�洢 ����
/**
 * ���յ�����1��ESPP8266����
 * �ж��Ƿ�ΪJSON������ִ����Ӧ�Ĺ���
 */
void Apps_Analysis_Data(void)
{
    uint8_t jsFirstOk[3];  //��һ�ν��յ�OK

    if (USART1_RX_STA & 0x8000)
    {
		json_get_ItemObjectFirst((char *)USART1_RX_BUF,"welcome",json_get_temp);
		if (json_check(json_get_temp)  == 1) //�Ϸ� Ϊ0  ���Ϸ�Ϊ1
		{
			json_get_value((char *)json_get_temp,"status",(char *)jsFirstOk);
		}else
		if(checkJSON("Connection: close"))
		{
			TIM_Cmd(TIM2, DISABLE);//���������
			Control_Flag_Mode=0;
			Dis_Number = 10;  //����Airkissģʽ
			//��Ч��+
			GPIO_ResetBits(GPIOA, GPIO_Pin_3); //1���1
			delay_ms(40);
			GPIO_SetBits(GPIOA, GPIO_Pin_3);//1���1
			delay_ms(40);
			GPIO_ResetBits(GPIOA, GPIO_Pin_3); //1���1
			delay_ms(40);
			GPIO_SetBits(GPIOA, GPIO_Pin_3);//1���1
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
 * �������ݵ�������
* ���͵�{"id":"SWTZ_Cantilever_a0:20:a6:29:a7:dc","to":"user","services":{"strength1":"0.001","threshold1":"132","threshold2":"123","strength2":"0.002"}}
 * ���յ���
 */
void send_data_user(void)
{
    char PRemove[32];
    uint8_t packingData[256];        //���800�ֽ�
	
	strcpy((char *) packingData, "{\"id\":\"SWTZ_Cantilever_");
    strcat((char *) packingData, macbufs);
    strcat((char *) packingData, "\",\"to\":\"user\",\"services\":{\"strength1\":\"");
		sprintf((char *)PRemove,"%d",strength1_status_value);  //strength1_status_value
    strcat((char *) packingData, (char *)PRemove);  //strength1��ֵ;
	
	strcat((char *) packingData, "\",\"threshold1\":\"");
		sprintf((char *)PRemove,"%d",strength1_check_value);  //strength1_status_value
	strcat((char *) packingData, (char *)PRemove);  //strength1��ֵ;
	
	strcat((char *) packingData, "\",\"strength2\":\"");
		sprintf((char *)PRemove,"%d",strength2_status_value);  //strength2_status_value
    strcat((char *) packingData, (char *)PRemove);  //strength1��ֵ;
	
	strcat((char *) packingData, "\",\"threshold2\":\"");
		sprintf((char *)PRemove,"%d",strength2_check_value);  //strength2_status_value
    strcat((char *) packingData, (char *)PRemove);  //strength1��ֵ;
	
	strcat((char *) packingData, "\"}}");
    sendMessagePacket((char *)packingData);
}


////////////********************** JSON  ���ݴ��� ***************************////////////////

/***************************************
 name:       JSON У�麯��
 input:      �ַ���
 output:     �Ϸ�JAON ����1 ���Ϸ�JSON ����0
 description:
 ***************************************/
char json_check(char *str)
{
    uint16_t str_length;//�����ַ����ȱ���
    str_length = strlen(str);//�����ַ�����
    if (str[0] == '{' && str[str_length - 1] == '}')  //ͨ����β�������ж��Ƿ�ΪJSON
    {
        return 1;//����ַ���Ϊ�Ϸ�JSON ����1
    }
    else
    {
        return 0;//����ַ���Ϊ�Ϸ�JSON ����0
    }
}

/***************************************
 name:       JSON ��ȡ��ֵ����
 input:      JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ���ַ���
 output:     �����ȡ�ɹ�����1 ��ȡʧ�ܷ���0
 description:
 ***************************************/
char json_get_value(char *json, char *json_key, char *json_value)
{
    char *json_key_start;//���������ʼ��λ��
    char *json_key_end;//�������������λ��
    char json_key_length;//�����������
    char *json_value_start;//�����ֵ��ʼ��λ��
    char *json_value_end;//�����ֵ������λ��
    char json_value_length;//�����ֵ����

    json_key_start = strstr(json, json_key);//��ȡ������ʼ��λ��
    json_key_length = strlen(json_key);//��ȡ�����ĳ���
    json_key_end = json_key_start + json_key_length;//��ȡ����������λ��
    if (json_key_start != 0 && *(json_key_start - 1) == '\"'
            && *(json_key_end) == '\"' && *(json_key_end + 1) == ':'
            && *(json_key_end + 2) == '\"')
    {
        json_value_start = json_key_end + 3;//��ȡ��ֵ��ʼ��λ��
        json_value_end = strstr(json_value_start, "\""); //��ȡ��ֵ������λ��
        json_value_length = json_value_end - json_value_start;//��ȡ��ֵ�ĳ���
        strncpy(json_value, json_value_start, json_value_length);//����ֵ����ָ������
        json_value[json_value_length] = '\0';//ָ�����һλ���ַ��������� \0
        return 1;//�ɹ���ȡ��ֵ ����1
    }
    else
    {
        json_value[0] = '\0';
        return 0;//ʧ�ܻ�ȡ��ֵ ����0
    }
}

/***************************************
 name:      JSON ��ȡ��JSON��ֵ��ֵ��һ����ȡ��
 input:     JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ��ֵ
 output:    ��
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
 name:      JSON ��ȡ��JSON��ֵ��ֵ
 input:     JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��ֵ��ֵ
 output:    ��
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
 name:      JSON ��ȡ��JSON(������ȡ)
 input:     JSON�ַ��� Ҫ��ȡ�ļ��� ��ȡ��json�Ĵ�
 output:    ��
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
 name:       JSON ��ֵ�ԱȺ���
 input:      JSON ��ֵ Ҫƥ����ַ�
 output:     ���ƥ��ɹ�����1 ʧ�ܷ���0
 description:
 ***************************************/
char json_check_value(char *str1, char *str2)
{
    if (strcmp(str1, str2) == 0)
    {
        return 1;//ƥ��ɹ�����1
    }
    else
    {
        return 0;//ƥ��ɹ�����0
    }
}


/***************************************
 name:       string����תint����
 input:      �洢string�Ĳ���
 output:     ���ת���ɹ�������ֵ ʧ�ܷ���0
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
 name:       �����յ��������Ƿ����ĳһ���ַ���
 input:      �洢string�Ĳ���
 output:     ���ת���ɹ�������ֵ ʧ�ܷ���0
 description:
 ***************************************/
char* checkJSON(char *str)
{
    char *strx=0;
    strx=strstr((const char*)USART1_RX_BUF,(const char*)str);
    return (char*)strx;
}

