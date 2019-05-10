#include "includes.h"

/**
 * websocket��ͨ������2���͵�ESP8266
 */

void printfResult(char *message,int counter)
{
    int i;
    for (i = 0; i < counter; i++)
    {
        USART_SendData(USART1,message[i]);
        while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//�ȴ����ͽ���
    }
}


/**
 * ����websocket���ݰ�
 */
void sendMessagePacket(char *message)
{
    char packet[300];
    unsigned long int length = 0;  // ���ݰ����ܳ���
    int counter = 0;  // ���ݵ�����
    int i;

    WebSocketFrame webSocketFrame;  //web�� �ṹ��
    webSocketFrame.fin = 1;  // 1 ��ʾ֡������ʾ  0����һ֡
    webSocketFrame.rsvs = 0;  //RSV1 RSV2 RSV3  ����0
    webSocketFrame.opcode = 0x01;  // 0X01��ʾ�ַ���
    webSocketFrame.masked = 1;     //�Ƿ�ʹ������ 1��ʹ��  0����ʹ��
    webSocketFrame.payloadlen = strlen(message);   //Ҫ���͵���Ϣ����
    webSocketFrame.maskkey[0] = 0x37;  //����
    webSocketFrame.maskkey[1] = 0xFA;  //����
    webSocketFrame.maskkey[2] = 0x21;  //����
    webSocketFrame.maskkey[3] = 0x3D;  //����

    length = webSocketFrame.payloadlen;  //������Ϣ�ĳ���

    packet[counter] = 0 | (webSocketFrame.fin << 7);   //����finλ���ֽڴ洢��packet[0]
    packet[counter] |= (webSocketFrame.opcode);    //����opcodeλ���ֽڴ洢��packet��
    counter++;
    packet[counter] = 0 | (webSocketFrame.masked << 7);  //����洢��packet��

    /**
    *������ݵĳ�����0-125֮�䣬����payload����ʵ����
    *������ݵĳ��ȵ���126С��65536��payloadlen=126��ʵ�ʳ��Ⱦ��Ǻ���2���ֽڣ���payload����ʵ����
    *������ݵĳ��ȴ���65535��payloadlen=127��ʵ�ʳ��Ⱦ��Ǻ���8���ֽڣ���payload����ʵ����
    */
    if (length < 126)  // ����ܳ���С��126
    {
        packet[counter] |= length;  // Payloadlength
        counter++;
        for (i = 0; i < 4; i++)
        {
            packet[counter] = webSocketFrame.maskkey[i];  //����������� ���ݼ���
            counter++;
        }
    }
    else if (length < 65536 && length > 125)      //������ȴ��ڵ���126����С��65536
    {
        packet[counter] |= 126;
        counter++;
        packet[counter] = (length >> 8) & 0xff;   //Payloadlength �����ʮ���Ƶ�10000(0X2710)������λ10011100010000 �������ֽ���Ҫ���ȷ��͸߰�λ���ڷ��͵Ͱ�λ(0X1027)
        counter++;
        packet[counter] = length & 0xff;
        counter++;
        for (i = 0; i < 4; i++)
        {
            packet[counter] = webSocketFrame.maskkey[i];   //����������� ���ݼ���
            counter++;
        }
    }
    //ԭʼ���ݵ�i���ֽڵ�ֵ XOR masking-key�ĵ�(i%4)���ֽڵ�ֵ
    for (int i = 0; i < length; i++)
    {
        uint8_t temp = 0;
        temp = (message[i] ^ webSocketFrame.maskkey[i % 4]); //������� ����������� ���ݼ���
        packet[counter] = temp;
        counter++;
    }
    printfResult((char *)packet,counter); // ����
}

