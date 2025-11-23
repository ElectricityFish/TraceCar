#include "stm32f10x.h"                  // Device header

//宏定义传感器返回值
#define Sensor_Left1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Sensor_Left2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define Sensor_Right1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define Sensor_Right2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

//宏定义行驶状态
#define Waiting 0                        //等待 
#define Direct 1                        //直行
#define LeftExcursion_Silght 2          //左轻微偏移
#define LeftExcursion_Large 3           //左大偏移
#define RightExcursion_Silght 4         //右轻微偏移
#define RightExcursion_Large 5          //右大偏移
#define LeftTurn_Arched 6               //左圆角转弯
#define LeftTurn_Vertical 7             //左直角转弯
#define RightTurn_Arched 8              //右圆角转弯
#define RightTurn_Vertical 9            //右直角转弯



void Sensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

}




uint8_t Trace_GetState(void)
{
    uint8_t L1 = Sensor_Left1;
    uint8_t L2 = Sensor_Left2;
    uint8_t R2 = Sensor_Right2; 
    uint8_t R1 = Sensor_Right1;
    
    // 1. 先判断特殊路口情况
    if(L1 == 0 && L2 == 0 && R2 == 0 && R1 == 0)return Direct; // 十字路口，直行通过 
    
    // 2. 直行 - 中间两个传感器在黑线上
    if(L1 == 1 && L2 == 0 && L2 == 0 && R1 == 1) return Direct;
    
    // 3. 转弯情况
    // 向左转弯
    if(L1 == 0 && L2 == 0 && R2 == 1 && R1 == 1) return LeftTurn_Arched;
    if(L1 == 0 && L2 == 0 && R2 == 0 && R1 == 1) return LeftTurn_Vertical;
    
    // 向右转弯  
    if(L1 == 1 && L2 == 1 && R2 == 0 && R1 == 0) return RightTurn_Arched;
    if(L1 == 1 && L2 == 0 && R2 == 0 && R1 == 0) return RightTurn_Vertical;
    
    // 4. 偏移情况
    // 左偏移
    if(L1==1 &&L2 == 1 && R2 == 1 && R1 == 0) return LeftExcursion_Large;
    if(L1 == 1 && L2 == 1 && R2 == 0 && R1 == 1) return LeftExcursion_Silght;
    
    // 右偏移
    if(L1 == 0 && L2 == 1 && R2 == 1&&R1==1) return RightExcursion_Large;
    if(L1 == 1 && L2 == 0 && R2 == 1 && R1 == 1) return RightExcursion_Silght;
    
    // 5. 最后判断等待状态 - 所有传感器都在白线上
    if(L1 == 1 && L2 == 1 && R2 == 1 && R1 == 1) return Waiting;
    
    return Waiting; // 默认返回等待
}







