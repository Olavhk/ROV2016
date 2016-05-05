/**
  **************************************************************************************
  * @file    h-file_template
  * @author  Olav Karstensen og Hartvik wakka wakka
  * @version V01
  * @date    03-Apr-2016
  * @brief   This file contains macros and extern function definitions for
  			 c-file_template.c.
  **************************************************************************************
  */

/* Macro -------------------------------------------------------------------------------*/
#define AUTO_MODE_1_ADC_SETUP							0x8000 // Configuring the Mode og the ADC
#define CONFIGURING_PORT_CHANNEL						0x0FFF // Configuring the channels in to the ADC
/*CONFIGURING THE Voltage settings*/
#define CONFIGURING_PORT								0x2C00 // Endre på 2 tallet for 5v
#define CURRENT_READ_MESAGE								0x0000 // Sending empty message to ADC to get reading back.

/* Exported function prototypes --------------------------------------------------------*/

/* Declare global functions here, preferrably with comments to explain the functions.
 */
extern void init_ADC_Current(void); // Init the ADC_Current_Chip
extern void init_SPI_1(void); 		// Init the SPI ant the GPIO_A_PIN 5,6,7
extern void ADC_Current(void); 		// Leser av strømmen
 void SPI_Send(uint16_t data); // Sende funksjon for å sende data på SPI
void init_SPI_1(void);
