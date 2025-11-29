#include "stm32f10x.h"                  // Device header

//宏定义传感器返回值
#define Sensor_LeftMid GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Sensor_Left GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define Sensor_RightMid GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define Sensor_Right GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

#define Sensor_FarLeft GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define Sensor_FarRight GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)

void Sensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_3;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
}


int16_t Sensor_Get_WeightError(void)//获取加权偏移误差
{

    int8_t L =Sensor_Left;
    int8_t L_Mid= Sensor_LeftMid;
    int8_t R_Mid =Sensor_RightMid; 
    int8_t R =Sensor_Right;
    int8_t FL=Sensor_FarLeft;
    int8_t FR=Sensor_FarRight;

    static int8_t Pre_Rescue=0;//检查是否要救车
    static int8_t Rescue_Count=0;
    if(FL==1)
    {
        Pre_Rescue=-1;
    }
    if(FR==1)
    {
        Pre_Rescue=1;
    }

   
    
    //L_Mid为-2，L权重为-8，FL为-20，R_Mid为2，R权重为8，FR为20
    int16_t Error;
    Error=(-2)*L_Mid+(-8)*L+(-20)*FL+2*R_Mid+8*R+20*FR;

    if(Pre_Rescue==-1&&Error==0)//向右完全脱离
    {
        Rescue_Count=-1;
    }

    if(Pre_Rescue==1&&Error==0)//向左完全脱离
    {
        Rescue_Count=1;
    }

    if(Rescue_Count==1)
    {
        if(L==1||R==1)//跳出救车
        {
            Pre_Rescue=0;
            Rescue_Count=0;
            return Error;
        }
        return 100;
    }

    if(Rescue_Count==-1)
    {
        if(L==1||R==1)//跳出救车
        {
            Pre_Rescue=0;
            Rescue_Count=0;
            return Error;
        }
        return -100;
    }
    
    return Error;
}

