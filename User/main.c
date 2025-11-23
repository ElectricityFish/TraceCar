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
#include "TraceTask.h"


//这里都是制动速度
int8_t DirectOut=100;
int8_t Motive_Excursion_Silght_Out=80,Motive_Excursion_Large_Out=60;
int8_t Motive_Turn_Arched_Out=80,Motive_Turn_Vertical_Out=60;

//以下是从动速度
int8_t Follow_Excursion_Silght_Out=-60,Follow_Excursion_Large_Out=-100;
int8_t Follow_Turn_Arched_Out=35,Follow_Turn_Vertical_Out=30;

uint8_t KeyNum,State;
uint8_t CarState;


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
		
		OLED_Update();
		if(State==0&&(KeyNum==2||KeyNum==3))//在调试模式下
		{
			if(KeyNum==2)Menu_Indxe++;
			if(KeyNum==3)Menu_Indxe--;

			
			if(Menu_Indxe>=9)Menu_Indxe=1;//一共8个可调参数
			if(Menu_Indxe<=0)Menu_Indxe=8;

			while(Key_GetNum()==0)//没有按键按下就进行调参
			{

				if(Menu_Indxe==1)
				{
					OLED_Printf(60,0,OLED_8X16,"M_ESO  ");
					OLED_Printf(0,20,OLED_8X16,"LESO:M=%03d,F=%03d",Motive_Excursion_Silght_Out,Follow_Excursion_Silght_Out);
				}
				if(Menu_Indxe==2)
				{
					OLED_Printf(60,0,OLED_8X16,"F_ESO  ");
					OLED_Printf(0,20,OLED_8X16,"LESO:M=%03d,F=%03d",Motive_Excursion_Silght_Out,Follow_Excursion_Silght_Out);
				}


				if(Menu_Indxe==3)
				{
					OLED_Printf(60,0,OLED_8X16,"M_ELO  ");
					OLED_Printf(0,20,OLED_8X16,"LELO:M=%03d,F=%03d",Motive_Excursion_Large_Out,Follow_Excursion_Large_Out);
				}				
				if(Menu_Indxe==4)
				{
					OLED_Printf(60,0,OLED_8X16,"F_ELO  ");
					OLED_Printf(0,20,OLED_8X16,"LELO:M=%03d,F=%03d",Motive_Excursion_Large_Out,Follow_Excursion_Large_Out);

				}

				if(Menu_Indxe==5)
				{
					OLED_Printf(60,0,OLED_8X16,"M_TAO  ");
					OLED_Printf(0,20,OLED_8X16,"LTAO:M=%03d,F=%03d",Motive_Turn_Arched_Out,Follow_Turn_Arched_Out);

				}				
				if(Menu_Indxe==6)
				{
					OLED_Printf(60,0,OLED_8X16,"F_TAO  ");
					OLED_Printf(0,20,OLED_8X16,"LTAO:M=%03d,F=%03d",Motive_Turn_Arched_Out,Follow_Turn_Arched_Out);
				}

				if(Menu_Indxe==7)
				{
					OLED_Printf(60,0,OLED_8X16,"M_TVO  ");
					OLED_Printf(0,20,OLED_8X16,"LTVO:M=%03d,F=%03d",Motive_Turn_Vertical_Out,Follow_Turn_Vertical_Out);

				}				
				if(Menu_Indxe==8)
				{
					OLED_Printf(60,0,OLED_8X16,"F_TVO  ");
					OLED_Printf(0,20,OLED_8X16,"LTVO:M=%03d,F=%03d",Motive_Turn_Vertical_Out,Follow_Turn_Vertical_Out);
				}

				

			

				if(Menu_Indxe==1)Motive_Excursion_Silght_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==2)Follow_Excursion_Silght_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==3)Motive_Excursion_Large_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==4)Follow_Excursion_Large_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==5)Motive_Turn_Arched_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==6)Follow_Turn_Arched_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==7)Motive_Turn_Vertical_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==8)Follow_Turn_Vertical_Out+=RotateEncoder_Get()*5;
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
				//CarState=Trace_GetState();
				//Motor_Set();
				TaskPerformance();
			}
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}


void Motor_Set(void)
{
	switch (CarState)
	{
		case Waiting:
		{
			Motor_SetPWM_Left1(60);
			Motor_SetPWM_Left2(60);
			Motor_SetPWM_Right1(60);
			Motor_SetPWM_Right2(60);
			break;
		}
		case Direct:
		{
			Motor_SetPWM_Left1(100);
			Motor_SetPWM_Left2(100);
			Motor_SetPWM_Right1(100);
			Motor_SetPWM_Right2(100);
			break;
		}
		case LeftExcursion_Silght:
		{
			Motor_SetPWM_Left1(Motive_Excursion_Silght_Out);
			Motor_SetPWM_Left2(Motive_Excursion_Silght_Out);
			Motor_SetPWM_Right1(Follow_Excursion_Silght_Out);
			Motor_SetPWM_Right2(Follow_Excursion_Silght_Out);
			break;
		}
		case LeftExcursion_Large:
		{
			Motor_SetPWM_Left1(Motive_Excursion_Large_Out);
			Motor_SetPWM_Left2(Motive_Excursion_Large_Out);
			Motor_SetPWM_Right1(Follow_Excursion_Large_Out);
			Motor_SetPWM_Right2(Follow_Excursion_Large_Out);
			break;
		}
		case RightExcursion_Silght:
		{
			Motor_SetPWM_Left1(Follow_Excursion_Silght_Out);
			Motor_SetPWM_Left2(Follow_Excursion_Silght_Out);
			Motor_SetPWM_Right1(Motive_Excursion_Silght_Out);
			Motor_SetPWM_Right2(Motive_Excursion_Silght_Out);
			break;
		}
		case RightExcursion_Large:
		{
			Motor_SetPWM_Left1(Follow_Excursion_Large_Out);
			Motor_SetPWM_Left2(Follow_Excursion_Large_Out);
			Motor_SetPWM_Right1(Motive_Excursion_Large_Out);
			Motor_SetPWM_Right2(Motive_Excursion_Large_Out);
			break;
		}
		case LeftTurn_Arched:
		{
			Motor_SetPWM_Left1(Follow_Turn_Arched_Out);
			Motor_SetPWM_Left2(Follow_Turn_Arched_Out);
			Motor_SetPWM_Right1(Motive_Turn_Arched_Out);
			Motor_SetPWM_Right2(Motive_Turn_Arched_Out);
			break;
		}
		case LeftTurn_Vertical:
		{
			Motor_SetPWM_Left1(Follow_Turn_Vertical_Out);
			Motor_SetPWM_Left2(Follow_Turn_Vertical_Out);
			Motor_SetPWM_Right1(Motive_Turn_Vertical_Out);
			Motor_SetPWM_Right2(Motive_Turn_Vertical_Out);
			break;
		}
		case RightTurn_Arched:
		{
			Motor_SetPWM_Left1(Motive_Turn_Arched_Out);
			Motor_SetPWM_Left2(Motive_Turn_Arched_Out);
			Motor_SetPWM_Right1(Follow_Turn_Arched_Out);
			Motor_SetPWM_Right2(Follow_Turn_Arched_Out);
			break;
		}
		case RightTurn_Vertical:
		{
			Motor_SetPWM_Left1(Motive_Turn_Vertical_Out);
			Motor_SetPWM_Left2(Motive_Turn_Vertical_Out);
			Motor_SetPWM_Right1(Follow_Turn_Vertical_Out);
			Motor_SetPWM_Right2(Follow_Turn_Vertical_Out);
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

