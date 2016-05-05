/**
  **************************************************************************************
  * @file    manip.h
  * @author  Simen W. Tofteberg
  * @version V01
  * @date    09-February-2016
  * @brief   This file contains macros and extern function definitions for
  			 manip.c.
  **************************************************************************************
  */
#include "stm32f10x.h" // h-file specific to the microcontroller. Allways include this.
#include "stm32f10x_can.h"  // h-file that declares functions from another c-file.
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "semihosting.h"

/* Macro -------------------------------------------------------------------------------*/
/* Put #define's here.
/* ID list, RANGE = [0...0x7FF] *********************************************************/
/*
#define CONSTANT_1							0x01
#define CONSTANT_2							300
*/
/* Exported function prototypes --------------------------------------------------------*/

/* Declare global functions here, preferrably with comments to explain the functions.
 */

//extern void Les_Enkodere(void); // Global function that does something.
//extern void Manip_Reguler(void);
extern void Manip_Oppstart(void);
//extern void Manip_Teleskop(uint8_t strom);
//extern void Manip_Grip(uint8_t strom);
extern void Manip_Grip_F(void);
