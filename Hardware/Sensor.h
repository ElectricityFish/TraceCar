#ifndef __SENSOR_H
#define __SENSOR_H

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

//循迹逻辑
uint8_t  Trace_GetState(void);

void Sensor_Init(void);
#endif
