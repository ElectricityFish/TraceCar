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

uint8_t KeyNum,State;
float kp=5.0,ki=0.0,kd=0.0;
int8_t DirectOut=80;
extern float Current_Error,Previous_Error,Error_Sum;

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
			Current_Error=0;
			Previous_Error=0;
			Error_Sum=0;
			//清除误差
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

		OLED_Printf(0,16,OLED_8X16,"Direct:%3d",DirectOut);
		OLED_Printf(0,32,OLED_8X16,"kp=%.1f,ki=%.1f",kp,ki);
		OLED_Printf(0,48,OLED_8X16,"kd=%.1f",kd);
		OLED_Update();
		if(State==0&&(KeyNum==2||KeyNum==3))//在调试模式下
		{
			if(KeyNum==2)Menu_Indxe++;
			if(KeyNum==3)Menu_Indxe--;
			if(Menu_Indxe>=5)Menu_Indxe=1;//一共4个可调参数
			if(Menu_Indxe<=0)Menu_Indxe=4;

			LED_OFF();
			while(Key_GetNum()==0)//没有按键按下就进行调参
			{
				if(Menu_Indxe==1)
				{
					OLED_Printf(60,0,OLED_8X16,"Direct  ");
				}
				if(Menu_Indxe==2)
				{
					OLED_Printf(60,0,OLED_8X16,"kp     ");
				}
				if(Menu_Indxe==3)
				{
					OLED_Printf(60,0,OLED_8X16,"ki     ");
				}
				if(Menu_Indxe==4)
				{
					OLED_Printf(60,0,OLED_8X16,"kd     ");
				}
				int8_t change=RotateEncoder_Get();		
				if(Menu_Indxe==1)DirectOut+=change;
				if(Menu_Indxe==2)kp+=(float)change*0.1;
				if(Menu_Indxe==3)ki+=(float)change*0.1;
				if(Menu_Indxe==4)kd+=(float)change*0.1;
				OLED_Printf(0,16,OLED_8X16,"Direct:%3d",DirectOut);
				OLED_Printf(0,32,OLED_8X16,"kp=%.1f,ki=%.1f",kp,ki);
				OLED_Printf(0,48,OLED_8X16,"kd=%.1f",kd);
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
		if(State==1)
		{
			LED_ON();
			PID_SetSpeed();
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}


