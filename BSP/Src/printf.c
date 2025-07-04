#include "printf.h"
#include <stdio.h>

// USART1��ʼ������
void USART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. ʹ��ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 2. ����GPIO����
    // PA9 - USART1_TX, PA10 - USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          // ���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // �������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    // ����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // ����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    // 4. ���ô��ڲ���
    USART_InitStructure.USART_BaudRate = 115200;          // ������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1λֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;    // ��У��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // �շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);

    // 5. �����жϣ���ѡ�����ڽ��գ�
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // ʹ�ܽ����ж�

    // 6. ʹ�ܴ���
    USART_Cmd(USART1, ENABLE);
}

// �ض���printf��USART1
int fputc(int ch, FILE *f) {
    // �ȴ��������
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    // �����ַ�
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}

// USART1�жϷ������������ã�
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART1); // ��ȡ���յ�������
        // ����������ݣ�������ԣ�
        USART_SendData(USART1, data); // ����
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

