#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "Motor.h"
#include <math.h>

extern int8_t DirectOut;
extern float kp,ki,kd;
float Current_Error,Previous_Error,Error_Sum;
float PID_Out;

void PID_SetSpeed(void)
{

    static uint8_t Count;
    Count++;
    if(Count>=50)
    {
        Count=0;//每50ms调控一次
        Previous_Error=Current_Error;
        Current_Error=Sensor_Get_WeightError();
        Error_Sum+=Current_Error;

        
        if(fabs(Current_Error<9))
        {
          Error_Sum+=Current_Error;
        }else{
            Error_Sum=+Current_Error/2;
        }

        float Error_Init=ki*Error_Sum;
        //积分限幅
        if(Error_Init>=40)Error_Init=40;
        if(Error_Init<=-30)Error_Init=-30;

        PID_Out=kp*Current_Error+Error_Init+kd*(Current_Error-Previous_Error);
        float Left_Out=DirectOut+PID_Out;
        float Right_Out=DirectOut-PID_Out;



        //输出限幅
        if(Left_Out>=65)Left_Out=65;
        if(Left_Out<=-50)Left_Out=-50;

        if(Right_Out>=85)Right_Out=65;
        if(Right_Out<=-50)Right_Out=-50;


        //电机调控
        Motor_SetPWM_Left1(Left_Out);
		Motor_SetPWM_Left2(Left_Out);
		Motor_SetPWM_Right1(Right_Out);
		Motor_SetPWM_Right2(Right_Out);

    }
}





