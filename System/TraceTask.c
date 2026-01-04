#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "Motor.h"
#include <math.h>
#include "Encoder.h"

extern int8_t DirectSpeed;
extern float kp,ki,kd;
float Current_Error,Previous_Error,Error_Sum;
float PID_Out;

float Speed_kp=1.7,Speed_ki=1.0,Speed_kd=0;

float Left_Target, Left_Actual, Left_Out;			//目标值，实际值，输出值

float Right_Target, Right_Actual, Right_Out;			//目标值，实际值，输出值

float Left_Integral = 0, Left_LastError = 0;
float Right_Integral = 0, Right_LastError = 0;


void PID_SetSpeed(void)
{

    //static uint8_t Count;
    //Count++;
    //if(Count>=50)
    //{
        //Count=0;//每50ms调控一次
        Previous_Error=Current_Error;
        Current_Error=Sensor_Get_WeightError();
        if(Current_Error==-100)//车辆向右出线
        {
          Left_Target=-20;
          Right_Target=30;
          return;
        }
        if(Current_Error==100)//车辆向左出线
        {
          Left_Target=30;
          Right_Target=-20;
          return;
        }

        Error_Sum+=Current_Error;

        float Error_Init=ki*Error_Sum;
        //积分限幅
        if(Error_Init>=20)Error_Init=20;
        if(Error_Init<=-20)Error_Init=-20;

        PID_Out=kp*Current_Error+Error_Init+kd*(Current_Error-Previous_Error);
        //PID限幅
        if(PID_Out>=60)PID_Out=60;
        if(PID_Out<=-60)PID_Out=-60;

        //更改目标速度
        Left_Target=DirectSpeed+PID_Out;
        Right_Target=DirectSpeed-PID_Out;



        //输出限幅
        if(Left_Target>=65)Left_Target=65;
        if(Left_Target<=-65)Left_Target=-65;

        if(Right_Target>=65)Right_Target=65;
        if(Right_Target<=-65)Right_Target=-65;
    //}
}


void PID_LeftMotorSpeed(void)
{
  
   
        
        Left_Actual = Encoder_Get_Left();
        float error = Left_Target - Left_Actual;
        
        // 位置式PID
        Left_Integral += error;
        // 抗积分饱和
        if(Left_Integral > 100) Left_Integral = 100;
        if(Left_Integral < -100) Left_Integral = -100;
        
        float derivative = error - Left_LastError;
        
        Left_Out = Speed_kp * error + Speed_ki * Left_Integral + Speed_kd * derivative;
        Left_LastError = error;
        
        // 输出限幅
        if(Left_Out > 100) Left_Out = 100;
        if(Left_Out < -100) Left_Out = -100;
        
        Motor_SetPWM_Left(Left_Out);
    
}


void PID_RightMotorSpeed(void)
{
  
  
        
        Right_Actual = Encoder_Get_Right();
        float error = Right_Target - Right_Actual;
        
        // 位置式PID
        Right_Integral += error;
        // 抗积分饱和
        if(Right_Integral > 100) Right_Integral = 100;
        if(Right_Integral < -100) Right_Integral = -100;
        
        float derivative = error - Right_LastError;
        
        Right_Out = Speed_kp * error + Speed_ki * Right_Integral + Speed_kd * derivative;
        Right_LastError = error;
        
        // 输出限幅
        if(Right_Out > 100) Right_Out = 100;
        if(Right_Out < -100) Right_Out = -100;
        
        Motor_SetPWM_Right(Right_Out);

}



