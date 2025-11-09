#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "AD.h"
#include "Sensor.h"

float kp=1.0,ki=1.0,kd=1.0;
int16_t DirectSpeed=250,MSmallTurnSpeed=180,MLargeTurnSpeed=120;//制动速度
int16_t SmallTuenSpeed=80,LargeTunrSpeed=-50;//转向速度
uint8_t KeyNum,State;

int main()
{
	Key_Init();
	OLED_Init();
	Motor_Init();
	Sensor_Init();
	LED_Init();
	AD_Init();
	Encoder_Init();
	Timer_Init();//一般定时器在最后初始化
	uint8_t Menu_Indxe=0;
	float AD_Value=0;//获得初始AD值
	float AD_PIDDebugChange=0;
	float AD_PIDDebug=0;//用于调试kp,ki,kd
	int16_t AD_SpeedDebug=0;//用于调试速度
	int16_t AD_SpeedDebugChange=0;//用于调试速度


	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)State=!State;
		if(State==0)OLED_Printf(0,0,OLED_8X16,"Debug");
		if(State==1)OLED_Printf(0,0,OLED_8X16,"RUN    ");
		OLED_Printf(0,20,OLED_6X8,"kp=%.2f  ki=%.2f",kp,ki);
		OLED_Printf(0,30,OLED_6X8,"kd=%.2f",kd);
		OLED_Printf(60,30,OLED_6X8,"DiS=%d",DirectSpeed);
		OLED_Printf(0,40,OLED_6X8,"MSTS=%d  MLTS=%d",MSmallTurnSpeed,MLargeTurnSpeed);
		OLED_Printf(0,50,OLED_6X8,"STS=%d  LTS=%d",SmallTuenSpeed,LargeTunrSpeed);
		OLED_Update();
		if(State==0&&KeyNum==2)//在调试模式下
		{
			Menu_Indxe++;
			if(Menu_Indxe>=9)Menu_Indxe=1;//一共8个可调参数
			if(Menu_Indxe==1||Menu_Indxe==2||Menu_Indxe==3)
			{
				if(Menu_Indxe==1)OLED_Printf(60,0,OLED_8X16,"kp  ");
				if(Menu_Indxe==2)OLED_Printf(60,0,OLED_8X16,"ki  ");
				if(Menu_Indxe==3)OLED_Printf(60,0,OLED_8X16,"kd  ");
				//消除刚进入菜单时的赋值
				uint8_t count=0;
				while(Key_GetNum()==0)//没有按键按下就进行调参
				{
					count++;
					AD_PIDDebugChange=AD_PIDDebug;
					AD_Value=(AD_GetValue_Median()*0.2+AD_Value*0.8);
					AD_PIDDebug=(AD_Value/4095)*3.0;
					AD_PIDDebugChange=AD_PIDDebug-AD_PIDDebugChange;
					if(count<100)AD_PIDDebugChange=0;
					if(Menu_Indxe==1)kp+=AD_PIDDebugChange;
					if(Menu_Indxe==2)ki+=AD_PIDDebugChange;
					if(Menu_Indxe==3)kd+=AD_PIDDebugChange;
					OLED_Printf(0,20,OLED_6X8,"kp=%.2f  ki=%.2f",kp,ki);
					OLED_Printf(0,30,OLED_6X8,"kd=%.2f",kd);
					OLED_Update();
					
				}
			}

			if(Menu_Indxe==4||Menu_Indxe==5||Menu_Indxe==6||Menu_Indxe==7||Menu_Indxe==8)
			{
				if(Menu_Indxe==4)OLED_Printf(60,0,OLED_8X16,"DiS  ");
				if(Menu_Indxe==5)OLED_Printf(60,0,OLED_8X16,"MSTS  ");
				if(Menu_Indxe==6)OLED_Printf(60,0,OLED_8X16,"MLTS  ");
				if(Menu_Indxe==7)OLED_Printf(60,0,OLED_8X16,"STS  ");
				if(Menu_Indxe==8)OLED_Printf(60,0,OLED_8X16,"LTS  ");
				//消除刚进入菜单时的赋值
				uint8_t count=0;
				while(Key_GetNum()==0)//没有按键按下就进行调参
				{
					count++;
					AD_SpeedDebugChange=AD_SpeedDebug;
					AD_Value=(AD_GetValue_Median()*0.1+AD_Value*0.8);
					AD_SpeedDebug=(AD_Value/4095)*200;
					AD_SpeedDebugChange=AD_SpeedDebug-AD_SpeedDebugChange;
					if(count<100)AD_SpeedDebugChange=0;
					if(Menu_Indxe==4)DirectSpeed+=AD_SpeedDebugChange;
					if(Menu_Indxe==5)MSmallTurnSpeed+=AD_SpeedDebugChange;
					if(Menu_Indxe==6)MLargeTurnSpeed+=AD_SpeedDebugChange;
					if(Menu_Indxe==7)SmallTuenSpeed+=AD_SpeedDebugChange;
					if(Menu_Indxe==8)LargeTunrSpeed+=AD_SpeedDebugChange;
					OLED_Printf(60,30,OLED_6X8,"DiS=%d",DirectSpeed);
					OLED_Printf(0,40,OLED_6X8,"MSTS=%d  MLTS=%d",MSmallTurnSpeed,MLargeTurnSpeed);
					OLED_Printf(0,50,OLED_6X8,"STS=%04d  LTS=%d",SmallTuenSpeed,LargeTunrSpeed);
					OLED_Update();
					
				}
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
