#ifndef __SENSOR_H
#define __SENSOR_H

//宏定义传感器返回值
#define Sensor_Left1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Sensor_Left2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define Sensor_Right1 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define Sensor_Right2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)

typedef enum{
    Direct,
    LeftTurn,
    RightTurn
}Sensor_Get;

//循迹逻辑
Sensor_Get  Trace_GetState(void);
void Sensor_Init(void);
#endif
