/*
 * SPI.h
 *
 *  Created on: 7 dic. 2020
 *      Author: Hanes
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdbool.h>
#include "main.h"

/// Comment this #define if you don't work with RTOS
#define NFC_SPI_RTOS

/// Time maximum to transmit data for SPI
#define SPI_NFC_TIMEOUT_TRANSMISSION	1000

/// Time maximum to wait reception data for SPI
#define SPI_NFC_TIMEOUT_RECEPTION		1000

/// Port and pin's number of Chip Select SPI
#define SPI_CS_Pin GPIO_PIN_11
#define SPI_CS_GPIO_Port GPIOA

/// Port and pin's number of IRQ to PN532 NFC
#define NFC_IRQ_Pin GPIO_PIN_6
#define NFC_IRQ_GPIO_Port GPIOH

/**
 * @brief Function to initialize peripheral SPI used to communicate between
 * NFC module and CPU.
 */
void nfc_spi2_set_nandler(SPI_HandleTypeDef *spi);

/**
 * @brief Receive single byte for SPI
 *
 * @return Received byte.
*/
uint8_t nfc_spi_get_byte(void);

/**
 * @brief Send single byte for SPI
 *
 * @param[in] byte Byte to be sent over SPI
 */
void nfc_spi_send_byte(const uint8_t byte);

/**
 * @brief Set SPI Chip enable signal. Setting the CS line will select the PN532,
 * true is selected and false is not selected.
 *
 * @param[in] state State of select signal, 1 to enable or other value to disable SPI
 */
void nfc_spi_set_select(const bool state);

/**
 *  @brief Test PN532 IRQ signal. The PN532 will toggle the IRQ to active
 *  when data is ready to be received from PN532
 *
 *  @return Return true if IRQ signal is active, false if not active.
 */
bool nfc_spi_get_irq(void);

#endif /* INC_SPI_H_ */
