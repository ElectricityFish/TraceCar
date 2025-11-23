#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "Motor.h"

/*
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
*/

//这里都是制动速度
extern int8_t DirectOut;
extern int8_t Motive_Excursion_Silght_Out,Motive_Excursion_Large_Out;
extern int8_t Motive_Turn_Arched_Out,Motive_Turn_Vertical_Out;
//以下是从动速度
extern int8_t Follow_Excursion_Silght_Out,Follow_Excursion_Large_Out;
extern int8_t Follow_Turn_Arched_Out,Follow_Turn_Vertical_Out;

//宏定义任务
#define Waiting 0
#define LeftExcursion_Correct 10
#define LeftOutLine_Correct 11
#define LeftTurning 12
#define RightExcursion_Correct 13
#define RightOutLine_Correct 14
#define RightTurning 15

uint8_t Current_SensorState;
uint8_t PreviousElement_SensorState=Waiting;//存放最近的特征状态,来判断车辆的行驶情况

uint8_t Motive_Task;//驱动任务

uint8_t Car_GetTask(void)
{
    PreviousElement_SensorState=Current_SensorState;
    Current_SensorState=Trace_GetState();

    if(Current_SensorState==Waiting&&PreviousElement_SensorState==Waiting)return Waiting;
    if(Current_SensorState==Direct)return Direct;
    //普通偏移
    if(Current_SensorState==LeftExcursion_Silght&&PreviousElement_SensorState==LeftExcursion_Silght)return LeftExcursion_Correct;
    if(Current_SensorState==RightExcursion_Silght&&PreviousElement_SensorState==RightExcursion_Silght)return RightExcursion_Correct;
    //之前轻微偏移，现在等待，说明刚好进入传感器盲区
    if(Current_SensorState==Waiting&&PreviousElement_SensorState==LeftExcursion_Silght)return LeftExcursion_Correct;
    if(Current_SensorState==Waiting&&PreviousElement_SensorState==RightExcursion_Silght)return RightExcursion_Correct;

    //处理出线，即所有传感器都在线的一侧
    if(Current_SensorState==Waiting&&PreviousElement_SensorState==LeftExcursion_Large)return LeftOutLine_Correct;
    if(Current_SensorState==Waiting&&PreviousElement_SensorState==RightExcursion_Large)return RightOutLine_Correct;

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







