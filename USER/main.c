#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include <stdio.h>       //C����ͷ�ļ�
#include "LCD.h"
#include "printf.h"
#include "stm32f4xx_tim.h"



#define IO_CS          (*((volatile unsigned short *) 0x60020000))     //MCU-IO��չģ���в���IOƬѡ��ַ
#define KEY_RAM               (*((volatile unsigned short *) 0x6006000C))     //���̽ӿڵ�ַ
#define FRE1_H        			(*((volatile unsigned short *) 0x60040002))       //0110_0000_0000_0100_0000_0000_0000_0000
#define FRE1_L        			(*((volatile unsigned short *) 0x60040000))       //0110_0000_0000_0100_0000_0000_0000_0010
#define FRE2_H        			(*((volatile unsigned short *) 0x60040006))       //0110_0000_0000_0100_0000_0000_0000_0100
#define FRE2_L        			(*((volatile unsigned short *) 0x60040004))       //0110_0000_0000_0100_0000_0000_0000_0110


// ��ȫ�ֱ����������(TIM4ʱ����������PB8)
volatile uint8_t pb8_toggle_enabled = 0;  // 0=ֹͣ, 1=����
volatile uint32_t tick_count = 0;        // ����������PB8�л���ʱ



// ���ò��� (�Ż���)
#define ADC_BUFFER_SIZE     4096         // �������� (���ٵ���)
uint16_t adcBuffer[ADC_BUFFER_SIZE];    // ADC���ݻ����� (����ΪFFT����)

#define ADC_SAMPLE_RATE  2000000.0f  // 2 MHz������
#define FFT_SIZE         4096        // FFT����
#define SIGNAL_MIN_FREQ  20000       // ��С�ź�Ƶ��(Hz)
#define SIGNAL_MAX_FREQ  100000      // ����ź�Ƶ��(Hz)

#ifndef PI
#define PI 3.14159265358979f
#endif




u8 keycode;
u8 keysign=0;
u16 i=0;

u16 tim_count=0;
volatile uint32_t interrupt_count = 0; // �жϼ���

void ADC_Config(void);
void TIM_Config(void);
void DMA_Config(void);
void TIM4_Init(void);


void FSMC_init(void);
void GPIO_Configuration(void);
void SysTick_Init(u32 clock);   //��ʱ��
void EXTI0_init(void);
void SystemInit1(void);


void delay_ms(volatile u16 time);
void SysTick_Init(u32 clock);  


int main(void)
{
 /*---------------����������ʼ��----------------*/
      FSMC_init();        
      LCD_Init9481(); //Һ����ʼ��                           
      GPIO_Configuration();                           //I/O��ʼ�� 

      SysTick_Init(210000);
      NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        
      EXTI0_init();                    //�ⲿ�жϳ�ʼ��

      
      
      
/*---------------------��ʾ�������õ�-----�������--------*/  
      //x-480  y-320
			LCD_ShowChineseStringBig(160,250,171,5,CYAN); //���ص���Դ
			

      LCD_ShowChineseStringBig(50,180,176,6,GREEN); //�������ֵ
      LCD_ShowChineseStringBig(118,130,182,4,GREEN); //����ֵ
    


      LCD_ShowStringBig(105, 50,"K0:+",GREEN);
      LCD_ShowStringBig(195, 50,"K1:-",GREEN);
      LCD_ShowStringBig(285, 50,"K2:step",GREEN);


		


/*---------------����������ʼ��---------------*/     
 
     TFTLED=0x01;       
     keysign=0; 
		 tim_count=0;
     
     
/*--------------------���ܺ�����ʼ��--------------------*/

		USART1_Init();			//���ڴ�ӡ
		TIM4_Init();
		    // �����ʼ��
    ADC_Config();
    TIM_Config();
    DMA_Config();
		

     
/*------------�������õ�--------------------------*/
     while(1)           
    { 
			//����㴫һ��Ƶ���ֺͲ�����ȷ��˫·DA�ܹ�ʹ��
			//����ADC��DMA�ܹ�ʹ��-------------debug
			//����FFT������Ƿ���Ч-------���ڴ�ӡ
			//��֤���
			//LCD_ShowNumBig(192,50,tim_count,CYAN);  //���ʱ�����ֵ


      if(keysign==1)                                      
      {
        keysign=0;  
          switch(keycode)
          {
            case 0x00:         					//k0��
            {
							
               
               break;	
            }
            case 0x01:         					//k1��
            {    
              
              
               break;	
            }
            case 0x02:         					//k2��
            {
               
              break;	
            }
            case 0x03:break;         					//k3��
            case 0x04:break;         					//k4��
            case 0x05:break;         					//k5��         
            case 0x06:break;         					//k6��         
            case 0x07:break;         					//k7��         
            case 0x08:break;         					//k8��            
            case 0x09:break;        					//k9��          
            case 0x0a:break;         					//ka��           
            case 0x0b:break;         					//kb��           
            case 0x0c:break;         					//kc��           
            case 0x0d:break;         					//kd��           
            case 0x0e:break;         					//ke��            
            case 0x0f:break;         					//kf��
          }
        }
      }

}






                     
void ADC_Config(void) {
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��ADC��GPIOʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    // ����ADC�������� (PA0 - ADC1ͨ��0)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // ADC��������
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;        // ��ͨ��
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // ����ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising; // �ⲿ����������
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; // TIM2����
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // ����ADCͨ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
    
    // ʹ��ADC
    ADC_Cmd(ADC1, ENABLE);
    
//    // У׼ADC
//    ADC_ResetCalibration(ADC1);
//    while(ADC_GetResetCalibrationStatus(ADC1));
//    ADC_StartCalibration(ADC1);
//    while(ADC_GetCalibrationStatus(ADC1));
}

void TIM_Config(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // ʹ��TIM2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    // ����TIM2��Ƶֵ (168MHzϵͳʱ��)
    uint32_t prescaler = (SystemCoreClock / (ADC_SAMPLE_RATE * 2)) - 1;
    
    // TIM2��������
    TIM_TimeBaseStructure.TIM_Period = 1;  // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    // ����TIM2�������
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1;  // 50%ռ�ձ�
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    
    // ����TIM2����Դ (TRGO)
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_OC3Ref);
    
    // ʹ��TIM2
    TIM_Cmd(TIM2, ENABLE);
}

void DMA_Config(void) {
    DMA_InitTypeDef DMA_InitStructure;
    
    // ʹ��DMA2ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
    // DMA����
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)adcBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ADC_BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // ����ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    
    // ʹ��DMA
    DMA_Cmd(DMA2_Stream0, ENABLE);
    
    // ����ADC��DMA
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
}


// TIM4�жϷ�����
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        
        // ÿ1000���жϣ�1�룩����һ�μ�����
        if (++interrupt_count >= 1000)
        {
            interrupt_count = 0;
            tim_count++; // �����������

        }
        // PB8�����Ա任����
        /*if(pb8_toggle_enabled)
        {
            tick_count += 1;  // ÿ���ж�����1ms
            
            if(tick_count >= 1000)  // 1����
            {
                tick_count = 0;
                GPIOB->ODR ^= GPIO_Pin_8;  // �л�PB8״̬
                LCD_ShowStringBig(50, 100, pb8_toggle_enabled ? "PB8 TOGGLING" : "PB8 STOPPED", 
                                 pb8_toggle_enabled ? RED : BLUE);
            }
        }*/
    }
}

// TIM4��ʼ������ (������)
void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    // 1. ʹ��TIM4ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    // 2. ����TIM4ʱ��Ƶ�� (84MHz)
    uint32_t tim4_clock = 84000000; // APB1ʱ��42MHz �� 2
    
    // 3. ������ʱ������Ŀ�꣺1ms�жϣ�
    uint16_t prescaler = 84 - 1;    // ��Ƶ��1MHz (84MHz/84=1MHz)
    uint16_t period = 1000 - 1;     // ����1000�� = 1ms (1us �� 1000)
    
    // 4. ����ʱ��
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    // 5. ʹ�ܸ����ж�
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    
    // 6. ����NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 7. ������ʱ��
    TIM_Cmd(TIM4, ENABLE);
}

void SysTick_Handler(void)    //ʱ�����
{
	    static int count = 0;
			if (count++ % 2 == 0) {  // ÿ2���жϷ�תһ��
        GPIO_ToggleBits(GPIOA, GPIO_Pin_9);
    }      
}  

void SysTick_Init(u32 clock)
{
    
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
		SysTick_Config(clock);
}


void SystemInit1(void) {
    // FPU���ã����ʹ�ø��㵥Ԫ��
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
    #endif

    // ��λRCC����
    RCC->CR |= RCC_CR_HSION;     // ����HSI
    while((RCC->CR & RCC_CR_HSIRDY) == 0);
    
    // ����ʱ��
}






void delay_ms(volatile u16 time)
{
	uint32_t start = SysTick->VAL;
		while(time--) {
			while((start - SysTick->VAL) < (SystemCoreClock/1000));
			start = SysTick->VAL;
		}
}


void FSMC_init(void)
{     
    FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;

    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); //ʹ��FSMCʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE); 

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //�����������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;//�ĳ�50MHz�������⣬�и����źţ����°�������

    //PORTD�����������
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);    //AD2
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);    //AD3
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);    //NOE
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);    //NWE
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);    //AD13
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);    //AD14
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);   //AD15
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);   //A16
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);   //A17
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);   //AD0
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);   //AD1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 |
                                                                          GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                                                          GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
                                                                           GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    //PORTE�����������
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);    //AD4
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);    //AD5
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);    //AD6
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);   //AD7
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);   //AD8
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);   //AD9
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);   //AD10
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);   //AD11
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);   //AD12
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | 
                                                                          GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
                                                                
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    //PORTB����������� 
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_FSMC);   //NDAV
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 5;//7
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 4;
    FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 15;//10
    FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
    //FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
    FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 2;//4
    FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
    FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_B;

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);      //ʹ��GPIOBʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);      //ʹ��GPIOCʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);      //ʹ��GPIOEʱ��
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             //PE0��PE1��PE2�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 | GPIO_Pin_2; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);     
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             //PC0��PC1��PC2����������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_0;    //
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_8;                 //PB1�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);   

    

    
}

void EXTI0_IRQHandler(void)
{
  keycode=KEY_RAM;
  keycode&=0x0f;
  keysign=1;
  EXTI_ClearITPendingBit(EXTI_Line0);
   
}
void EXTI0_init(void)
{
  
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //Enable GPIOB clock 
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);// Enable SYSCFG clock 
      
      GPIO_InitTypeDef GPIO_InitStructure;
      EXTI_InitTypeDef EXTI_InitStructure;
      NVIC_InitTypeDef NVIC_InitStructure;
      
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);             //Connect EXTI Line0 to PB pin 
      
      EXTI_InitStructure.EXTI_Line = EXTI_Line0;/* Configure EXTI Line0 */
      EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_Init(&EXTI_InitStructure);
      EXTI_ClearFlag(EXTI_Line0);
      
      /* Enable and set EXTI Line0 Interrupt to the lowest priority */
      NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
}


