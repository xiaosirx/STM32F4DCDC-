#include "pwm.h"

void Pwm_Init(u16 psc, u16 arr)
{

	GPIO_InitTypeDef GpioIniture;
	TIM_TimeBaseInitTypeDef TimIniture;
	TIM_OCInitTypeDef OcIniture;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GpioIniture.GPIO_Mode  = GPIO_Mode_AF;
	GpioIniture.GPIO_OType = GPIO_OType_PP;
	GpioIniture.GPIO_Pin   = GPIO_Pin_3;
	GpioIniture.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GpioIniture.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GpioIniture);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);
	
	TimIniture.TIM_ClockDivision	 = TIM_CKD_DIV1;
	TimIniture.TIM_CounterMode		 = TIM_CounterMode_Up;
	TimIniture.TIM_Period			 = arr;
	TimIniture.TIM_Prescaler		 = psc;
	TimIniture.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TimIniture);
	
	OcIniture.TIM_OCIdleState  = TIM_OCIdleState_Reset;
	OcIniture.TIM_OCMode	   = TIM_OCMode_PWM1;
	OcIniture.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	OcIniture.TIM_OCNPolarity  = TIM_OCPolarity_High;
	OcIniture.TIM_OCPolarity   = TIM_OCNPolarity_High;
	OcIniture.TIM_OutputNState = DISABLE;
	OcIniture.TIM_OutputState  = ENABLE;
	OcIniture.TIM_Pulse		   = 0;
	TIM_OC4Init(TIM2, &OcIniture);
	
	TIM_OC4PreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);
	
}
