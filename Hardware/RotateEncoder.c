#include "stm32f10x.h"

int16_t RotateEncoder_Count = 0;

void RotateEncoder_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    // AFIO配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource15);
    
    // EXTI配置 - 修正线号
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line14 | EXTI_Line15;  // 修正为14和15
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStruct);
    
    // NVIC配置 - 修正中断通道
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;  // PC14和PC15共享这个中断
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStruct);
}

// 修正中断服务函数名
void EXTI15_10_IRQHandler(void)
{
    // 处理PC14中断 (EXTI_Line14)
    if(EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0)
        {
            RotateEncoder_Count--;
        }
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    
    // 处理PC15中断 (EXTI_Line15)
    if(EXTI_GetITStatus(EXTI_Line15) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0)
        {
            RotateEncoder_Count++;
        }
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
}

int16_t RotateEncoder_Get(void)
{
    int16_t Temp = RotateEncoder_Count;
    RotateEncoder_Count = 0;
    return Temp;
}

