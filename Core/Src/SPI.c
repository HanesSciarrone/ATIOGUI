/*
 * SPI.c
 *
 *  Created on: 7 dic. 2020
 *      Author: Hanes
 */

#include "SPI.h"
#ifdef NFC_SPI_RTOS
#include "cmsis_os.h"
#endif

static SPI_HandleTypeDef *spi_nfc;

/* Global function implementation -------------------------------------------- */
void nfc_spi2_set_nandler(SPI_HandleTypeDef *spi)
{
	spi_nfc = spi;
}

uint8_t nfc_spi_get_byte(void)
{
	uint8_t byte = 0x00;

#ifdef NFC_SPI_RTOS
	while (HAL_SPI_GetState(spi_nfc) != HAL_SPI_STATE_READY) 			//Is possble receive?
	{
		osDelay(1/portTICK_PERIOD_MS);
	}
	HAL_SPI_Receive(spi_nfc, &byte, 1, SPI_NFC_TIMEOUT_RECEPTION);
#else
	while (HAL_SPI_GetState(spi_nfc) != HAL_SPI_STATE_READY); 			//Is possble receive?
	HAL_SPI_Receive(spi_nfc, &byte, 1, SPI_NFC_TIMEOUT_RECEPTION);
#endif

	return byte;
}

void nfc_spi_send_byte(const uint8_t byte)
{
#ifdef NFC_SPI_RTOS
	while (HAL_SPI_GetState(spi_nfc) != HAL_SPI_STATE_READY) 					//Is possble transmit?
	{
		osDelay(1/portTICK_PERIOD_MS);
	}
	HAL_SPI_Transmit(spi_nfc, (uint8_t *)&byte, 1, SPI_NFC_TIMEOUT_TRANSMISSION);	// send 8 bits of data
	while (HAL_SPI_GetState(spi_nfc) == HAL_SPI_STATE_BUSY)						//Transmission ready?
	{
		osDelay(1/portTICK_PERIOD_MS);
	}
#else
	while (HAL_SPI_GetState(spi_nfc) != HAL_SPI_STATE_READY); 						//Is possble transmit?
	HAL_SPI_Transmit(spi_nfc, (uint8_t *)&byte, 1, SPI_NFC_TIMEOUT_TRANSMISSION);	// send 8 bits of data
	while (HAL_SPI_GetState(spi_nfc) == HAL_SPI_STATE_BUSY);						//Transmission ready?
#endif
}

void nfc_spi_set_select(const bool state)
{
	if (state)
	{
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
	}
}

bool nfc_spi_get_irq(void)
{
	if ( HAL_GPIO_ReadPin(NFC_IRQ_GPIO_Port, NFC_IRQ_Pin) == GPIO_PIN_SET )
	{
		return false;
	}

	return true;
}
