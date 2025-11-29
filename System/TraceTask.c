#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "Motor.h"
#include <math.h>
#include "Encoder.h"

extern int8_t DirectSpeed;
extern float kp,ki,kd;
float Current_Error,Previous_Error,Error_Sum;
float PID_Out;

float Speed_kp=1.0,Speed_ki=0.5,Speed_kd=0;

float Left_Target, Left_Actual, Left_Out;			//目标值，实际值，输出值
float Left_Error0, Left_Error1, Left_Error2;		//本次误差，上次误差，上上次误差

float Right_Target, Right_Actual, Right_Out;			//目标值，实际值，输出值
float Right_Error0, Right_Error1, Right_Error2;		//本次误差，上次误差，上上次误差

void PID_SetSpeed(void)
{

    static uint8_t Count;
    Count++;
    if(Count>=50)
    {
        Count=0;//每50ms调控一次
        Previous_Error=Current_Error;
        Current_Error=Sensor_Get_WeightError();
        if(Current_Error==-100)//车辆向右出线
        {
          Left_Target=0;
          Right_Target=40;
          return;
        }
        if(Current_Error==100)//车辆向左出线
        {
          Left_Target=40;
          Right_Target=0;
          return;
        }

        Error_Sum+=Current_Error;

        float Error_Init=ki*Error_Sum;
        //积分限幅
        if(Error_Init>=40)Error_Init=40;
        if(Error_Init<=-30)Error_Init=-30;

        PID_Out=kp*Current_Error+Error_Init+kd*(Current_Error-Previous_Error);
        //更改目标速度
        Left_Target=DirectSpeed+PID_Out;
        Right_Target=DirectSpeed-PID_Out;



        //输出限幅
        if(Left_Target>=65)Left_Target=65;
        if(Left_Target<=-65)Left_Target=-65;

        if(Right_Target>=65)Right_Target=65;
        if(Right_Target<=-65)Right_Target=-65;
    }
}

void PID_LeftMotorSpeed(void)
{

  static uint8_t Count1;
  Count1++;
  if(Count1>=10)//每10ms进行一次速度调控
  {
    Count1=0;
    Left_Actual=Encoder_Get_Left();
    Left_Error2 = Left_Error1;			//获取上上次误差
		Left_Error1 = Left_Error0;			//获取上次误差
		Left_Error0 = Left_Target - Left_Actual;	//获取本次误差，目标值减实际值，即为误差值
			
			/*PID计算*/
			/*使用增量式PID公式，计算得到输出值*/
			Left_Out += Speed_kp * (Left_Error0 - Left_Error1) + Speed_ki* Left_Error0
					+ Speed_kd * (Left_Error0 - 2 * Left_Error1 + Left_Error2);
			
			/*输出限幅*/
			if (Left_Out > 100) {Left_Out = 100;}		//限制输出值最大为100
			if (Left_Out < -100) {Left_Out = -100;}	//限制输出值最小为100
      Motor_SetPWM_Left(Left_Out);
  }


}

void PID_RightMotorSpeed(void)
{

  static uint8_t Count2;
  Count2++;
  if(Count2>=10)//每10ms进行一次速度调控
  {
    Count2=0;
    Right_Actual=Encoder_Get_Right();
    Right_Error2 = Right_Error1;			//获取上上次误差
		Right_Error1 = Right_Error0;			//获取上次误差
		Right_Error0 = Right_Target - Right_Actual;	//获取本次误差，目标值减实际值，即为误差值
			
			/*PID计算*/
			/*使用增量式PID公式，计算得到输出值*/
			Right_Out += Speed_kp * (Right_Error0 - Right_Error1) + Speed_ki* Right_Error0
					+ Speed_kd * (Right_Error0 - 2 * Right_Error1 + Right_Error2);
			
			/*输出限幅*/
			if (Right_Out > 100) {Right_Out = 100;}		//限制输出值最大为100
			if (Right_Out < -100) {Right_Out = -100;}	//限制输出值最小为100
      Motor_SetPWM_Right(Right_Out);
  }


}





