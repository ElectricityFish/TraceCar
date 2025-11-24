#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "Motor.h"

float kp=0.2,ki=0,kd=0;
float Current_Error,Previous_Error,Error_Sum;
float PID_Out;

float DirectOut=60;
void PID_SetSpeed(void)
{

    static uint8_t Count;
    Count++;
    if(Count>=40)
    {
        Count=0;//每40ms调控一次
        Previous_Error=Current_Error;
        Current_Error=Sensor_Get_WeightError();
        Error_Sum+=Current_Error;

        //积分限幅
        if(Error_Sum>=60)Error_Sum=60;
        if(Error_Sum<=-60)Error_Sum=-60;

        PID_Out=kp*Current_Error+ki*Error_Sum+kd*(Current_Error-Previous_Error);
        float Left_Out=DirectOut+PID_Out;
        float Right_Out=DirectOut-PID_Out;
        //输出限幅
        if(Left_Out>=100)Left_Out=60;
        if(Left_Out<=-100)Left_Out=-40;

        if(Right_Out>=100)Right_Out=60;
        if(Right_Out<=-100)Right_Out=-40;

        Motor_SetPWM_Left1(Left_Out);
		Motor_SetPWM_Left2(Left_Out);
		Motor_SetPWM_Right1(Right_Out);
		Motor_SetPWM_Right2(Right_Out);

    }
}





