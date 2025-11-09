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

	while(1)
	{



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
