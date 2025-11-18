#include "stm32f10x.h"                  // Device header

#include "Motor.h"
#include "Encoder.h"

float kp,ki,kd;

void Left1Control(int  Target)
{
    static float Error0, Error1, Error2;		//本次误差，上次误差，上上次误差
    static float  Actual, Out;			        //实际值，输出值

    Actual= Encoder_Get_Left1();
    Error2=Error1;
    Error1=Error0;
    Error0=Target-Actual;

    /*PID计算*/
			/*使用增量式PID公式，计算得到输出值*/
			Out += kp * (Error0 - Error1) + ki * Error0+ kd * (Error0 - 2 * Error1 + Error2);
			/*输出限幅*/
			if (Out > 100) {Out = 100;}		//限制输出值最大为100
			if (Out < -100) {Out = -100;}	//限制输出值最小为100


    Motor_SetPWM_Left1(Out);
}

void Left2Control(int  Target)
{
    static float Error0, Error1, Error2;		//本次误差，上次误差，上上次误差
    static float  Actual, Out;			        //实际值，输出值

    Actual= Encoder_Get_Left1();
    Error2=Error1;
    Error1=Error0;
    Error0=Target-Actual;

    /*PID计算*/
			/*使用增量式PID公式，计算得到输出值*/
			Out += kp * (Error0 - Error1) + ki * Error0+ kd * (Error0 - 2 * Error1 + Error2);
			/*输出限幅*/
			if (Out > 100) {Out = 100;}		//限制输出值最大为100
			if (Out < -100) {Out = -100;}	//限制输出值最小为100


    Motor_SetPWM_Left2(Out);
}

void Right1Control(int  Target)
{
    static float Error0, Error1, Error2;		//本次误差，上次误差，上上次误差
    static float  Actual, Out;			        //实际值，输出值

    Actual= Encoder_Get_Left1();
    Error2=Error1;
    Error1=Error0;
    Error0=Target-Actual;

    /*PID计算*/
			/*使用增量式PID公式，计算得到输出值*/
			Out += kp * (Error0 - Error1) + ki * Error0+ kd * (Error0 - 2 * Error1 + Error2);
			/*输出限幅*/
			if (Out > 100) {Out = 100;}		//限制输出值最大为100
			if (Out < -100) {Out = -100;}	//限制输出值最小为100


    Motor_SetPWM_Right1(Out);
}

void Right2Control(int  Target)
{
    static float Error0, Error1, Error2;		//本次误差，上次误差，上上次误差
    static float  Actual, Out;			        //实际值，输出值

    Actual= Encoder_Get_Left1();
    Error2=Error1;
    Error1=Error0;
    Error0=Target-Actual;

    /*PID计算*/
			/*使用增量式PID公式，计算得到输出值*/
			Out += kp * (Error0 - Error1) + ki * Error0+ kd * (Error0 - 2 * Error1 + Error2);
			/*输出限幅*/
			if (Out > 100) {Out = 100;}		//限制输出值最大为100
			if (Out < -100) {Out = -100;}	//限制输出值最小为100


    Motor_SetPWM_Right2(Out);
}





