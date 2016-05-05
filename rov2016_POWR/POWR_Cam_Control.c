
/**
**************************************************************************************
* @file    POWR_Cam_Control.c
* @author  Olav H Karstensen
* @version V1.0
* @date    9-Mars-2016
* @brief   To Control the angle of the Cam.
**************************************************************************************
*/

/* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
* To control the Camera over the Can bus.
* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
*/
/* Include------------------------------------------------------------------------------*/

/* Include .h files from other modules here.*/

#include "stm32f10x_gpio.h" // Gpio oppsett
#include "stm32f10x.h"    // h-file specific to the microcontroller. Allways include this.
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "POWR_Switch.h"

/* Global variables --------------------------------------------------------------------*/
/* If needed, include header file for extern declarations of global variables here.
* Try to avoid global variables. Using getters and setters is usually a better
* alternative.
*/


/* Private Function Prototypes ----------------------------------------------------------*/

	/* Put prototypes for static functions here.
	* Private/static functions can only be called from within this c-file. Use static
	* for all functions that does not need to be used by an outside program.
	*/

/* Private variables -------------------------------------------------------------------*/
/* Put static/private variables here. These variables are only accessible from this
* .c-file.
*/
//	static uint8_t dimming ;



/*Public Functions --------------------------------------------------------------------*/


/* @brief  Init the Cam_control PWM GPIPA 8
*/
extern void Cam_Control_Init(void){
	// Settte opp GPIO pinnene til Mosfet styrings signalene.
	//Konfiguering
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // For TIM1
	TIM_OCInitTypeDef TIM_OCInitStructure; // For TIM_OC
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // Klokketilgang til AFIO-perifermodul

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //Alternativ funksjon. Klokkemodulen styrer GPIOen
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


	// Initialisering, laste ned kofigurasjonen til modulen
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // Gi tilgang til klokken
	// Innstillinger for TIM1
	/* Dette gir ~100Hz  */
	TIM_TimeBaseStructure.TIM_Period = 1000000/100 - 100; // ~100 Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 72; // Preskalering av TIM klokke
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // Sette clock divider
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Telleretning
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); // Laster ned innstillingene

	// Innstillinger for TIM_OC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM modus
	/* Aktiverer sammenligning med verdien i Capture Compare Register */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //
	TIM_OCInitStructure.TIM_Pulse = ((1000000/100)-100)/2; // 50% duty cycle.
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // Polaritet

	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	/* TIM4 aktiveres ikke før kjør-kommando mottas.Se SysTick_Handler().*/
	TIM_Cmd(TIM1, ENABLE);
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, DISABLE);


} //  Cam_Control_Init END

/**
* @brief  To turn the Motorswitch , Light, CoolingFan ;Mosfet ON\OFF
* @param  uint8_t parameter: 0 = Off 0 < ON
* @retval No return
*/
extern void setCAM_POS(void){
	TIM1->CCR1 = 3217;
	GPIOC->ODR ^= GPIO_Pin_13; // Flip receive-LED.
	//Cam PWM
//	if(Cam_message & Dimming_Value){
//		pos = (message & Dimming_Value) >>3 ;
//
//		if(dimming<=10) TIM4->CCR3 = (2475 + 742*pos);
//	}
}	//END setCAN_POS
