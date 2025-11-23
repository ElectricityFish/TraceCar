#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "Motor.h"

/*
typedef enum{
    Waiting,
    Direct,
    LeftExcursion_Silght,
    LeftExcursion_Large,
    RightExcursion_Silght,
    RightExcursion_Large,
    LeftTurn_Arched,
    LeftTurn_Vertical,
    RightTurn_Arched,
    RightTurn_Vertical
}Sensor_Get;
*/
/*
//这里都是制动速度
extern int8_t DirectOut;
extern int8_t Motive_Excursion_Silght_Out,Motive_Excursion_Large_Out;
extern int8_t Motive_Turn_Arched_Out,Motive_Turn_Vertical_Out;
//以下是从动速度
extern int8_t Follow_Excursion_Silght_Out,Follow_Excursion_Large_Out;
extern int8_t Follow_Turn_Arched_Out,Follow_Turn_Vertical_Out;

//枚举任务
typedef enum
{
    Waiting,
    Direct,
    LeftExcursion_Correct ,
    LeftOutLine_Correct ,
    LeftTurning ,
    RightExcursion_Correct, 
    RightOutLine_Correct ,
    RightTurning 
}Car_Task;


Sensor_Get Current_SensorState=Static;
Sensor_Get PreviousElement_SensorState=Static;//存放最近的特征状态,来判断车辆的行驶情况

uint8_t Motive_Task;//驱动任务

Car_Task Car_GetTask(void)
{
    PreviousElement_SensorState=Current_SensorState;
    Current_SensorState=Trace_GetState();

    if(Current_SensorState==Static&&PreviousElement_SensorState==Static)return Waiting;
    if(Current_SensorState==Direct_Sensor)return Direct;
    //普通偏移
    if(Current_SensorState==LeftExcursion_Silght&&PreviousElement_SensorState==LeftExcursion_Silght)return LeftExcursion_Correct;
    if(Current_SensorState==RightExcursion_Silght&&PreviousElement_SensorState==RightExcursion_Silght)return RightExcursion_Correct;
    //之前轻微偏移，现在等待，说明刚好进入传感器盲区
    if(Current_SensorState==Static&&PreviousElement_SensorState==LeftExcursion_Silght)return LeftExcursion_Correct;
    if(Current_SensorState==Static&&PreviousElement_SensorState==RightExcursion_Silght)return RightExcursion_Correct;

    //处理出线，即所有传感器都在线的一侧
    if(Current_SensorState==Static&&PreviousElement_SensorState==LeftExcursion_Large)return LeftOutLine_Correct;
    if(Current_SensorState==Static&&PreviousElement_SensorState==RightExcursion_Large)return RightOutLine_Correct;

    return Direct;
}

void TaskPerformance(void)//返回表示是否在执行某一任务
{
    switch (Car_GetTask())
    {
        case Direct:
	{
		Motor_SetPWM_Left1(DirectOut);
		Motor_SetPWM_Left2(DirectOut);
		Motor_SetPWM_Right1(DirectOut);
		Motor_SetPWM_Right2(DirectOut);
		break;
	}

    case LeftExcursion_Correct:
    {
        while(1)
        {
            Motor_SetPWM_Left1(Motive_Excursion_Silght_Out);
            Motor_SetPWM_Left2(Motive_Excursion_Silght_Out);
            Motor_SetPWM_Right1(Follow_Excursion_Silght_Out);
            Motor_SetPWM_Right2(Follow_Excursion_Silght_Out);
            if(Car_GetTask()==Direct) break;//完成校正
        }
        
		break;
    }
    case RightExcursion_Correct:
    {
        while(1)
        {
            Motor_SetPWM_Left1(Follow_Excursion_Silght_Out);
            Motor_SetPWM_Left2(Follow_Excursion_Silght_Out);
            Motor_SetPWM_Right1(Motive_Excursion_Silght_Out);
            Motor_SetPWM_Right2(Motive_Excursion_Silght_Out);
            if(Car_GetTask()==Direct) break;//完成校正
        }
        
		break;
    }

    case LeftOutLine_Correct:
    {
        while(1)
        {
            Motor_SetPWM_Left1(Motive_Excursion_Large_Out);
            Motor_SetPWM_Left2(Motive_Excursion_Large_Out);
            Motor_SetPWM_Right1(Follow_Excursion_Large_Out);
            Motor_SetPWM_Right2(Follow_Excursion_Large_Out);
            if(Car_GetTask()==Direct) break;//完成校正
        }
        
		break;
    }
    case RightOutLine_Correct:
    {
        while(1)
        {
             Motor_SetPWM_Left1(Follow_Excursion_Large_Out);
            Motor_SetPWM_Left2(Follow_Excursion_Large_Out);
            Motor_SetPWM_Right1(Motive_Excursion_Large_Out);
            Motor_SetPWM_Right2(Motive_Excursion_Large_Out);
            if(Car_GetTask()==Direct) break;//完成校正
        }
       
		break;
    }
    case Waiting:
    {
        Motor_SetPWM_Left1(0);
		Motor_SetPWM_Left2(0);
		Motor_SetPWM_Right1(0);
		Motor_SetPWM_Right2(0);
		break;
    }
    default:
    {
        Motor_SetPWM_Left1(DirectOut);
		Motor_SetPWM_Left2(DirectOut);
		Motor_SetPWM_Right1(DirectOut);
		Motor_SetPWM_Right2(DirectOut);
		break;
    }
    }
    

}

*/





