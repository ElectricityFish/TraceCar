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
int8_t LeftExcursion_Silght_Out=100,LeftExcursion_Large_Out=85;
int8_t RightExcursion_Silght_Out=100,RightExcursion_Large_Out=85;
int8_t LeftTurn_Arched_Out=80,LeftTurn_Vertical_Out=70;
int8_t RightTurn_Arched_Out=80,RightTurn_Vertical_Out=70;
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
		OLED_Printf(0,20,OLED_6X8,"LESO=%03d  LELO=%03d",LeftExcursion_Silght_Out,LeftExcursion_Large_Out);
		OLED_Printf(0,30,OLED_6X8,"RESO=%03d RELO=%03d",RightExcursion_Silght_Out,RightExcursion_Large_Out);
		OLED_Printf(0,40,OLED_6X8,"LTAO=%03d  LTVO=%03d",LeftTurn_Arched_Out,LeftTurn_Vertical_Out);
		OLED_Printf(0,50,OLED_6X8,"RTAO=%03d  RTVO=%03d",RightTurn_Arched_Out,RightTurn_Vertical_Out);
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
			if(Count>=40)
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
		case Static:
		{
			OLED_Printf(60,0,OLED_6X8,"Static           ");
			OLED_Update();
			Motor_SetPWM_Left1(0);
			Motor_SetPWM_Left2(0);
			Motor_SetPWM_Right1(0);
			Motor_SetPWM_Right2(0);
			break;
		}
		case Direct:
		{
			OLED_Printf(60,0,OLED_6X8,"Direct          ");
			OLED_Update();
			Motor_SetPWM_Left1(100);
			Motor_SetPWM_Left2(100);
			Motor_SetPWM_Right1(100);
			Motor_SetPWM_Right2(100);
			break;
		}
		case LeftExcursion_Silght:
		{
			OLED_Printf(60,0,OLED_6X8,"LeftExcur_Silght             ");
			OLED_Update();
			Motor_SetPWM_Left1(LeftExcursion_Silght_Out);
			Motor_SetPWM_Left2(LeftExcursion_Silght_Out);
			Motor_SetPWM_Right1(70);
			Motor_SetPWM_Right2(70);
			break;
		}
		case LeftExcursion_Large:
		{
			OLED_Printf(60,0,OLED_6X8,"LeftExcu_Large        ");
			OLED_Update();
			Motor_SetPWM_Left1(LeftExcursion_Large_Out);
			Motor_SetPWM_Left2(LeftExcursion_Large_Out);
			Motor_SetPWM_Right1(50);
			Motor_SetPWM_Right2(50);
			break;
		}
		case RightExcursion_Silght:
		{
			OLED_Printf(60,0,OLED_6X8,"RightExcu_Silght         ");
			OLED_Update();
			Motor_SetPWM_Left1(70);
			Motor_SetPWM_Left2(70);
			Motor_SetPWM_Right1(RightExcursion_Silght_Out);
			Motor_SetPWM_Right2(RightExcursion_Silght_Out);
			break;
		}
		case RightExcursion_Large:
		{
			OLED_Printf(60,0,OLED_6X8,"RightExcu_Large");
			OLED_Update();
			Motor_SetPWM_Left1(50);
			Motor_SetPWM_Left2(50);
			Motor_SetPWM_Right1(RightExcursion_Large_Out);
			Motor_SetPWM_Right2(RightExcursion_Large_Out);
			break;
		}
		case LeftTurn_Arched:
		{
			OLED_Printf(60,0,OLED_6X8,"LeftTurn_Arched         ");
			OLED_Update();
			Motor_SetPWM_Left1(50);
			Motor_SetPWM_Left2(50);
			Motor_SetPWM_Right1(LeftTurn_Arched);
			Motor_SetPWM_Right2(LeftTurn_Arched);
			break;
		}
		case LeftTurn_Vertical:
		{
			OLED_Printf(60,0,OLED_6X8,"LeftTurn_Vertical          ");
			OLED_Update();
			Motor_SetPWM_Left1(35);
			Motor_SetPWM_Left2(35);
			Motor_SetPWM_Right1(LeftTurn_Vertical_Out);
			Motor_SetPWM_Right2(LeftTurn_Vertical_Out);
			break;
		}
		case RightTurn_Arched:
		{
			OLED_Printf(60,0,OLED_6X8,"RightTurn_Arched      ");
			OLED_Update();
			Motor_SetPWM_Left1(RightTurn_Arched);
			Motor_SetPWM_Left2(RightTurn_Arched);
			Motor_SetPWM_Right1(50);
			Motor_SetPWM_Right2(50);
			break;
		}
		case RightTurn_Vertical:
		{
			OLED_Printf(60,0,OLED_6X8,"RightTurn_Vertical     ");
			OLED_Update();
			Motor_SetPWM_Left1(RightTurn_Vertical_Out);
			Motor_SetPWM_Left2(RightTurn_Vertical_Out);
			Motor_SetPWM_Right1(35);
			Motor_SetPWM_Right2(35);
			break;
		}
		default :
		{
			OLED_Printf(60,0,OLED_6X8,"Other         ");
			OLED_Update();
			Motor_SetPWM_Left1(0);
			Motor_SetPWM_Left2(0);
			Motor_SetPWM_Right1(0);
			Motor_SetPWM_Right2(0);
		}
		




	}
	
	
}
/*
int8_t DirectOut=100;
int8_t LeftExcursion_Silght_Out,LeftExcursion_Large_Out;
int8_t RightExcursion_Silght_Out,RightExcursion_Large_Out;
int8_t LeftTurn_Arched_Out,LeftTurn_Vertical_Out;
int8_t RightTurn_Arched_Out,RightTurn_Vertical_Out;
uint8_t KeyNum,State;
uint8_t CarState;
*/

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
