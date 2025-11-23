#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "Motor.h"
#include "Serial.h"
#include "Sensor.h"
#include "RotateEncoder.h"
//#include "TraceTask.h"


//这里都是制动速度
int8_t DirectOut=100;
int8_t Motive_Turn_Out=80;
//以下是从动速度
int8_t Follow_TurnOut=-60;


uint8_t KeyNum,State;
Sensor_Get CarState;

int main()
{
	Key_Init();
	OLED_Init();
	Motor_Init();
	Sensor_Init();
	LED_Init();
	RotateEncoder_Init();
	Timer_Init();//一般定时器在最后初始化
	uint8_t Menu_Indxe=0;
	


	//Motor_SetPWM_Left1(40);
	//Motor_SetPWM_Left2(40);
	//Motor_SetPWM_Right1(40);
	//Motor_SetPWM_Right2(40);

	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)State=!State;
		if(State==0)
		{
			Motor_SetPWM_Left1(0);
			Motor_SetPWM_Left2(0);
			Motor_SetPWM_Right1(0);
			Motor_SetPWM_Right2(0);
			OLED_Printf(0,0,OLED_8X16,"Debug                                    ");
		}
		if(State==1)OLED_Printf(0,0,OLED_8X16,"RUN                                      ");
		/********************************************* 
		 * 
		 * 
		 * 简易菜单
		 * 
		 * 
		 * 
		*********************************************/

		OLED_Printf(0,16,OLED_8X16,"Direct:%d",DirectOut);
		OLED_Printf(0,32,OLED_8X16,"MotiveTurn=%d",Motive_Turn_Out);
		OLED_Printf(0,48,OLED_8X16,"FollowTurn:%d",Follow_TurnOut);
		
		OLED_Update();
		if(State==0&&(KeyNum==2||KeyNum==3))//在调试模式下
		{
			if(KeyNum==2)Menu_Indxe++;
			if(KeyNum==3)Menu_Indxe--;
			if(Menu_Indxe>=4)Menu_Indxe=1;//一共3个可调参数
			if(Menu_Indxe<=0)Menu_Indxe=3;

			while(Key_GetNum()==0)//没有按键按下就进行调参
			{
				if(Menu_Indxe==1)
				{
					OLED_Printf(60,0,OLED_8X16,"Direct  ");
				}
				if(Menu_Indxe==2)
				{
					OLED_Printf(60,0,OLED_8X16,"M_Turn  ");
				}
				if(Menu_Indxe==3)
				{
					OLED_Printf(60,0,OLED_8X16,"F_Turn  ");
				}				
				if(Menu_Indxe==1)DirectOut+=RotateEncoder_Get()*5;
				if(Menu_Indxe==2)Motive_Turn_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==3)Follow_TurnOut+=RotateEncoder_Get()*5;
				OLED_Printf(0,16,OLED_8X16,"Direct:%d",DirectOut);
				OLED_Printf(0,32,OLED_8X16,"MotiveTurn=%d",Motive_Turn_Out);
				OLED_Printf(0,48,OLED_8X16,"FollowTurn:%d",Follow_TurnOut);
				OLED_Update();
			}


			
		}


}
}

void Motor_Set(void);

void TIM1_UP_IRQHandler(void)
{
	static uint8_t Count;
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		
		Key_Tick();
		if(State==1)
		{
			Count++;
			if(Count>=20)
			{
				Count=0;
				CarState=Trace_GetState();
				Motor_Set();
			}
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

void Motor_Set(void)
{
	switch (CarState)
	{
		case Direct:
		{
			Motor_SetPWM_Left1(DirectOut);
			Motor_SetPWM_Left2(DirectOut);
			Motor_SetPWM_Right1(DirectOut);
			Motor_SetPWM_Right2(DirectOut);
			break;
		}
		case LeftTurn:
		{
			Motor_SetPWM_Left1(Follow_TurnOut);
			Motor_SetPWM_Left2(Follow_TurnOut);
			Motor_SetPWM_Right1(Motive_Turn_Out);
			Motor_SetPWM_Right2(Motive_Turn_Out);
			break;
		}
		case RightTurn:
		{
			Motor_SetPWM_Left1(Motive_Turn_Out);
			Motor_SetPWM_Left2(Motive_Turn_Out);
			Motor_SetPWM_Right1(Follow_TurnOut);
			Motor_SetPWM_Right2(Follow_TurnOut);
			break;
		}
		default :
		{
			Motor_SetPWM_Left1(60);
			Motor_SetPWM_Left2(60);
			Motor_SetPWM_Right1(60);
			Motor_SetPWM_Right2(60);
			break;
		}

	}
	
}

