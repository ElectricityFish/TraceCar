#include "stm32f10x.h"                  // Device header

//宏定义传感器返回值
#define Sensor_Left1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Sensor_Left2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define Sensor_Right1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define Sensor_Right2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define Sensor_FarLeft GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define Sensor_FarRight GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)

void Sensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}


int16_t Sensor_Get_WeightError(void)//获取加权偏移误差
{
    //取反将黑线变为1
    int8_t L = !Sensor_Left1;
    int8_t L_Mid = !Sensor_Left2;
    int8_t R_Mid = !Sensor_Right2; 
    int8_t R = !Sensor_Right1;
    int8_t FL=!Sensor_FarLeft;
    int8_t FR=!Sensor_FarRight;
    
    //L_Mid为-1，L权重为-3，FL为-9，R_Mid为1，R权重为3，FR为9
    int16_t Error;
    Error=(-1)*L_Mid+(-3)*L+(-9)*FL+R_Mid+3*R+9*FR;
    
    return Error;
}

