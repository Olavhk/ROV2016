/*#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


void TIM4_dutyCycle(uint8_t dimming);
* init_TIM4()
 @breif Initialize TIM4 as a PWM output.
 	 Output pin PB8.
 	 The p.w.m. is to change the brightness of the LED on the ROV.
 	 We need a Voltage divider to secure the dimming module.(Type:LDU1416S1000)
 	-

extern void init_TIM4(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // For TIM4
	TIM_OCInitTypeDef TIM_OCInitStructure; // For TIM_OC
	GPIO_InitTypeDef GPIO_InitStructure; // for GPIOD

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // Gi tilgang til klokken

	// Innstillinger for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); // Gi tilgang til klokken
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // Pin PB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternativ funksjon
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 10MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); // Laster ned innstillingene

	// Innstillinger for TIM4
	* Dette gir ~100Hz
	TIM_TimeBaseStructure.TIM_Period = 1000000/100 - 100; // ~100 Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 72; // Preskalering av TIM klokke
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // Sette clock divider
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Telleretning
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); // Laster ned innstillingene

	// Innstillinger for TIM_OC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM modus
	* Aktiverer sammenligning med verdien i Capture Compare Register
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //
	TIM_OCInitStructure.TIM_Pulse = ((1000000/100)-100)/2; // 50% duty cycle.
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // Polaritet

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);
	* TIM4 aktiveres ikke før kjør-kommando mottas.Se SysTick_Handler().
	TIM_Cmd(TIM4, ENABLE);
} // end TIM4_init()


* setTIM4_dutyCycle()
* @brief  Dimmming the light on the ROV, Changing the duty cycle
* @param  uint8_t dimming goes from 0-10 ---> 25% -100% Dimming of the light
*
*

extern void setTIM4_dutyCycle(uint8_t dimming){

if(dimming<=10) TIM4->CCR3 = (2475 + 742*dimming);

}
*/
