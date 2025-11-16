#include "stm32f10x.h"                  // Device header

//宏定义传感器返回值
#define Sensor_Left1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Sensor_Left2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define Sensor_Right1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define Sensor_Right2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

//宏定义行驶状态
#define Static 0                        //等待 
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
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD ;//改为默认低电平的下拉输入
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

}


//循迹逻辑
uint8_t  Trace_GetState(void)
{
    //等待
    if(Sensor_Left2==0&&Sensor_Right2==0&&Sensor_Left1==0&&Sensor_Right1==0)return Static;
    //直行
    if((Sensor_Left2==1&&Sensor_Right2==1)&&((Sensor_Left1==1&&Sensor_Right1==1)||(Sensor_Left1==0&&Sensor_Right1==0)))return Direct;
    //偏移校准
    if((Sensor_Left1==0&&Sensor_Right2==1)&&(Sensor_Left1==0&&Sensor_Right1==0))return LeftExcursion_Silght;
    if((Sensor_Left1==0&&Sensor_Right2==0)&&(Sensor_Left1==0&&Sensor_Right1==1))return LeftExcursion_Large;

    if((Sensor_Left1==0&&Sensor_Right2==1)&&(Sensor_Left1==0&&Sensor_Right1==0))return RightExcursion_Silght;
    if((Sensor_Left1==0&&Sensor_Right2==0)&&(Sensor_Left1==0&&Sensor_Right1==1))return RightExcursion_Large;
    //转向
    if((Sensor_Left2==1&&Sensor_Left1==1)&&(Sensor_Right2==0&&Sensor_Right1==0))return LeftTurn_Arched;
    if((Sensor_Left2==1&&Sensor_Left1==1)&&(Sensor_Right2==1&&Sensor_Right1==0))return LeftTurn_Vertical;

    if((Sensor_Left2==0&&Sensor_Left1==0)&&(Sensor_Right2==1&&Sensor_Right1==1))return RightTurn_Arched;
    if((Sensor_Left2==1&&Sensor_Left1==0)&&(Sensor_Right2==1&&Sensor_Right1==1))return RightTurn_Vertical;


    //其它情况待定
    return Static;

}







