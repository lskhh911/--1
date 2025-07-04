#include "printf.h"
#include <stdio.h>

// USART1初始化函数
void USART1_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 1. 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // 2. 配置GPIO引脚
    // PA9 - USART1_TX, PA10 - USART1_RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          // 复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    // 高速
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          // 上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 引脚复用映射
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

    // 4. 配置串口参数
    USART_InitStructure.USART_BaudRate = 115200;          // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;    // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_Init(USART1, &USART_InitStructure);

    // 5. 配置中断（可选，用于接收）
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断

    // 6. 使能串口
    USART_Cmd(USART1, ENABLE);
}

// 重定向printf到USART1
int fputc(int ch, FILE *f) {
    // 等待发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    // 发送字符
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}

// USART1中断服务函数（接收用）
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        uint8_t data = USART_ReceiveData(USART1); // 读取接收到的数据
        // 处理接收数据（例如回显）
        USART_SendData(USART1, data); // 回显
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}

