/*
 * SPI.c
 *
 *  Created on: Dec 20, 2020
 *      Author: Hanes
 */

#include "SPI.h"
#ifdef NFC_SPI_RTOS
#include "cmsis_os.h"
#endif

#define SPI_CS_Pin GPIO_PIN_11
#define SPI_CS_GPIO_Port GPIOA
#define NFC_IRQ_Pin GPIO_PIN_6
#define NFC_IRQ_GPIO_Port GPIOH

/* ---------------------- Private global variable --------------------- */
static SPI_HandleTypeDef *nfc_spi = NULL;


/* ------------------- Implementation public method ------------------- */
void handler_spi_init(SPI_HandleTypeDef *hspi)
{
	if (hspi == NULL) {
		return;
	}

	nfc_spi = hspi;
}

uint8_t spi_get_byte(void)
{
	uint8_t byte = 0x00;

#ifdef NFC_SPI_RTOS
	while (HAL_SPI_GetState(nfc_spi) != HAL_SPI_STATE_READY) {			//Is possble receive?
		osDelay(1/portTICK_PERIOD_MS);
	}
	HAL_SPI_Receive(nfc_spi, &byte, 1, SPI_NFC_TIMEOUT_RECEPTION);
#else
	while (HAL_SPI_GetState(nfc_spi) != HAL_SPI_STATE_READY); 			//Is possble receive?
	HAL_SPI_Receive(nfc_spi, &byte, 1, SPI_NFC_TIMEOUT_RECEPTION);
#endif

	return byte;
}

void spi_send_byte(uint8_t byte)
{
#ifdef NFC_SPI_RTOS
	while (HAL_SPI_GetState(nfc_spi) != HAL_SPI_STATE_READY) {						//Is possble transmit?
		osDelay(1/portTICK_PERIOD_MS);
	}
	HAL_SPI_Transmit(nfc_spi, (uint8_t *)&byte, 1, SPI_NFC_TIMEOUT_TRANSMISSION);	// send 8 bits of data
	while (HAL_SPI_GetState(nfc_spi) == HAL_SPI_STATE_BUSY) {						//Transmission ready?
		osDelay(1/portTICK_PERIOD_MS);
	}
#else
	while (HAL_SPI_GetState(nfc_spi) != HAL_SPI_STATE_READY); 						//Is possble transmit?
	HAL_SPI_Transmit(nfc_spi, (uint8_t *)&byte, 1, SPI_NFC_TIMEOUT_TRANSMISSION);	// send 8 bits of data
	while (HAL_SPI_GetState(nfc_spi) == HAL_SPI_STATE_BUSY);						//Transmission ready?
#endif
}

void spi_set_select(bool state)
{
	if (state) {
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
	}
	else {
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
	}
}

bool spi_get_irq(void)
{
	if (HAL_GPIO_ReadPin(NFC_IRQ_GPIO_Port, NFC_IRQ_Pin) == GPIO_PIN_SET) {
		return false;
	}

	return true;
}
