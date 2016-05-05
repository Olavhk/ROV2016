/**
 ******************************************************************************
 * @file    SysTick_metoder.c
 * @author  Sivert Sliper and Stian Sørensen
 * @version V1.0
 * @date    08-February-2016
 * @brief   This file contains all the functions prototypes for the SysTick
 *          timer.
 *
 ******************************************************************************
 */

/* Include---- ------------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_gpio.h" // Gpio oppsett
#include "stm32f10x_can.h"
#include "POWR_Canbus.h"
#include "POWR_rov2016_Interface.h"
#include "POWR_declare_global_vars.h"
#include "POWR_Switch.h"
//#include "math.h"

/* Global variables --------------------------------------------------------------------*/
//#include "extern_decl_global_vars.h"

/* Private variables -------------------------------------------------------------------*/

//static uint8_t isActive = 0;
static uint8_t counter_10_hz = 0;
static uint16_t counter_1_hz = 0;
static uint8_t message = 0;
static int32_t depth_data = 0;
//static uint8_t old_message = 0;
uint8_t can_receive_buffer[16][8] = {0};

/* Private function declarations -------------------------------------------------------*/

/* Macro -------------------------------------------------------------------------------*/
//#define DEBUG_MODE

/* Function definitions ----------------------------------------------------------------*/

/**
 * @brief  Configures the SysTick timer for 100 Hz interrupt frequency.
 * @param  None
 * @retval None
 */
extern void SysTick_init(void) {
	SysTick->CTRL = 0; /* Disable SysTick */
	SysTick->LOAD = 72000000/100;  // 10 msek avbruddsintervall.
	SysTick->VAL = 0;
	SysTick->CTRL = (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk
			| SysTick_CTRL_CLKSOURCE_Msk);
} // end Systick_init()


/**
 * @brief  Configures the CAN-Controller peripheral for 500 kbps communication.
 * 		   Also configures Rx filters according to ID's specified in "can_metoder.h"
 * @param  None
 * @retval None
 */
void SysTick_Handler(void){
	counter_10_hz++;
	counter_1_hz++;
	/* 100Hz loop*/

	/* Mottak av CAN-meldinger skjer her!. */
	if(CAN_GetFlagStatus(CAN1, CAN_FLAG_FMP0)){
		CanRxMsg rxmsg;
		CAN_Receive(CAN1, CAN_FIFO0, &rxmsg);
		uint8_t rx_length = rxmsg.DLC;
		if (rx_length == 0) return; // Return if message is empty.
		do{
			can_receive_buffer[rxmsg.FMI][rx_length-1] = rxmsg.Data[rx_length-1];
		}while(--rx_length>0);
		/* Indicating message received.*/
		GPIOC->ODR ^= GPIO_Pin_13; // Flip receive-LED.

		message = can_receive_buffer[fmi_topside_powr_ctrl][0];
		//deth = can_receive_buffer(fmi_sensor_depth_temp[]);
//		uint8_t* depth_pointer;
//		depth_pointer = CAN_getMessagePointer(fmi_sensor_depth_temp);
//
//		depth_data = (int32_t) (
//					((uint32_t)*depth_pointer++) << 24
//					|((uint32_t)*depth_pointer++) << 16
//					|((uint32_t)*depth_pointer++) << 8
//					|((uint32_t)*depth_pointer)
//					);

		setPOWR_UPDATE(message);

	} // end CAN-mottak.

//	message = CAN_getByteFromMessage(fmi_topside_powr_ctrl,0);
//
//	if(message != old_message){
//
//		setPOWR_UPDATE(message);
//		old_message = message;
//
//
//	}

	/* 10 Hz loop */
	if((counter_10_hz>9)){
		FlagStatus_SysTick = 1;


		counter_10_hz = 0;
	} // end 10 hz loop.

	/* 1 Hz loop */
	if(counter_1_hz>99){
		CAN_transmitAlive();
		counter_1_hz = 0;

	}// end 1 Hz loop.
} // end Systick_Handler()







