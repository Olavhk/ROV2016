/**
 **************************************************************************************
 * @file    c-fil_template.c
 * @author  Olav H Karstensen
 * @version V1.0
 * @date    19-Mars-2016
 * @brief   To read Current ADS79xx-Q1
 **************************************************************************************
 */

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "POWR_Current.h"
#include "stm32f10x_spi.h"

/**Private variables*/

static uint8_t i;
static uint16_t current[12];

void init_SPI_1(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA,
			ENABLE);
	/* SPIy Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	/* Configure SPI pins: SCK and MOSI ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	/* Configure SCK and MOSI pins as Alternate Function Push Pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	// clock speed
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure SPI pins: MISO ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	/* Configure MISO pin as Input Floating  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure SPICS ---------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable SPI1 */
	SPI_Cmd(SPI1, ENABLE);

}

void init_ADC_Current(void) {

	/* Send SPI1 data */
	SPI_Send(CONFIGURING_PORT);

	/* Send SPI1 data */
	SPI_Send(AUTO_MODE_1_ADC_SETUP);
		;
	/* Send SPI1 data */
	SPI_Send(CONFIGURING_PORT_CHANNEL);
		;
//	/* Send SPI1 data */
//	SPI_Send(CONFIGURING_PORT);
}

void ADC_Current(void) {
	for (i = 0; i <= 11; i++) {
		/* Send SPI1 data */
		SPI_Send(CURRENT_READ_MESAGE);
		//Current vector
		current[i] = SPI_I2S_ReceiveData(SPI1);
	}
}

void SPI_Send(uint16_t data){

	//Select low
	GPIOA->ODR &= ~GPIO_Pin_4;
	//Sending data
	SPI_I2S_SendData(SPI1, data);
	/* Wait for SPI1 Tx buffer empty */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	//Select high
	GPIOA->ODR |= GPIO_Pin_4;

}

/* SPI_send_data(uint16_t data)
 * CS lav
 * SPI_I2S_SendData(data);
 * vent
 * CS høy
 *
 */
