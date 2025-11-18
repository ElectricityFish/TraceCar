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


int8_t DirectOut=100;
int8_t LeftExcursion_Silght_Out,LeftExcursion_Large_Out;
int8_t RightExcursion_Silght_Out,RightExcursion_Large_Out;
int8_t LeftTurn_Arched_Out,LeftTurn_Vertical_Out;
int8_t RightTurn_Arched_Out,RightTurn_Vertical_Out;
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
	


	//Motor_SetPWM_Left1(100);
	//Motor_SetPWM_Left2(100);
	//Motor_SetPWM_Right1(100);
	//Motor_SetPWM_Right2(100);

	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)State=!State;
		if(State==0)OLED_Printf(0,0,OLED_8X16,"Debug                         ");
		if(State==1)OLED_Printf(0,0,OLED_8X16,"RUN                           ");
		/********************************************* 
		 * 
		 * 
		 * 简易菜单
		 * 
		 * 
		 * 
		*********************************************/
		OLED_Printf(0,20,OLED_6X8,"LESO=%04d  LELO=%04d",LeftExcursion_Silght_Out,LeftExcursion_Large_Out);
		OLED_Printf(0,30,OLED_6X8,"RESO=%04d RELO=%04d",RightExcursion_Silght_Out,RightExcursion_Large_Out);
		OLED_Printf(0,40,OLED_6X8,"LTAO=%04d  LTVO=%04d",LeftTurn_Arched_Out,LeftTurn_Vertical_Out);
		OLED_Printf(0,50,OLED_6X8,"RTAO=%04d  RTVO=%04d",RightTurn_Arched_Out,RightTurn_Vertical_Out);
		OLED_Update();
		if(State==0&&KeyNum==2)//在调试模式下
		{
			Menu_Indxe++;
			if(Menu_Indxe>=9)Menu_Indxe=1;//一共8个可调参数

			if(Menu_Indxe==1)OLED_Printf(60,0,OLED_8X16,"LESO  ");
			if(Menu_Indxe==2)OLED_Printf(60,0,OLED_8X16,"LELO  ");
			if(Menu_Indxe==3)OLED_Printf(60,0,OLED_8X16,"RESO  ");
			if(Menu_Indxe==4)OLED_Printf(60,0,OLED_8X16,"RELO  ");
			if(Menu_Indxe==5)OLED_Printf(60,0,OLED_8X16,"LTAO  ");
			if(Menu_Indxe==6)OLED_Printf(60,0,OLED_8X16,"LTVO  ");
			if(Menu_Indxe==7)OLED_Printf(60,0,OLED_8X16,"RTAO  ");
			if(Menu_Indxe==8)OLED_Printf(60,0,OLED_8X16,"RTVO  ");
		
			while(Key_GetNum()==0)//没有按键按下就进行调参
			{
				if(Menu_Indxe==1)LeftExcursion_Silght_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==2)LeftExcursion_Large_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==3)RightExcursion_Silght_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==4)RightExcursion_Large_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==5)LeftTurn_Arched_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==6)LeftTurn_Vertical_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==7)RightTurn_Arched_Out+=RotateEncoder_Get()*5;
				if(Menu_Indxe==8)RightTurn_Vertical_Out+=RotateEncoder_Get()*5;


				OLED_Printf(0,20,OLED_6X8,"LESO=%04d  LELO=%04d",LeftExcursion_Silght_Out,LeftExcursion_Large_Out);
				OLED_Printf(0,30,OLED_6X8,"RESO=%04d RELO=%04d",RightExcursion_Silght_Out,RightExcursion_Large_Out);
				OLED_Printf(0,40,OLED_6X8,"LTAO=%04d  LTVO=%04d",LeftTurn_Arched_Out,LeftTurn_Vertical_Out);
				OLED_Printf(0,50,OLED_6X8,"RTAO=%04d  RTVO=%04d",RightTurn_Arched_Out,RightTurn_Vertical_Out);
				
				OLED_Update();
					
			}


			
		}


}
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
