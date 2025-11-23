#include "stm32f10x.h"                  // Device header

//宏定义传感器返回值
#define Sensor_Left1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Sensor_Left2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define Sensor_Right1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define Sensor_Right2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

//枚举行驶状态
typedef enum{
    Direct,
    LeftTurn,
    RightTurn
}Sensor_Get;

void Sensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
}

Sensor_Get Trace_GetState(void)
{
    uint8_t L = Sensor_Left1;
    uint8_t L2 = Sensor_Left2;
    uint8_t R2 = Sensor_Right2; 
    uint8_t Mid;
    if(L2==0||R2==0)
    {
        Mid=0;
    }else{
        Mid=1;
    }
    
    uint8_t R = Sensor_Right1;
    
    // 1，直行
    if(L == 0 && Mid==0&& R == 0)return Direct; // 十字路口，直行通过     
    if(L == 1 && Mid==0 && R == 1) return Direct;  
    if(L==0&&Mid==1&&R==0)return  Direct;
    if(L=1&&Mid==1&&R==1)return Direct;
    // 2,左转情况
    if(L==0&&Mid==0&&R==1)return LeftTurn;
    if(L==0&&Mid==1&&R==1)return LeftTurn;
    //3，右转
    if(L==1&&Mid==0&&R==0)return LeftTurn;
    if(L==1&&Mid==1&&R==0)return LeftTurn;
    
}

