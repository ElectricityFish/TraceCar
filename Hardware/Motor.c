#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	//初始化4路电机
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	PWM_Init();
}





void Motor_SetPWM_Left1(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		PWM_SetCompare_Left1(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		PWM_SetCompare_Left1(-Speed);
	}
}

void Motor_SetPWM_Left2(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
		PWM_SetCompare_Left2(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
		PWM_SetCompare_Left2(-Speed);
	}
}


void Motor_SetPWM_Right1(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		PWM_SetCompare_Right1(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		PWM_SetCompare_Right1(-Speed);
	}
}


void Motor_SetPWM_Right2(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		PWM_SetCompare_Right2(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		PWM_SetCompare_Right2(-Speed);
	}
}
