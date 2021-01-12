/*
 * SPI.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Hanes
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdbool.h>
#include "stm32f7xx_hal.h"

/// Comment this #define if you don't work with RTOS
#define NFC_SPI_RTOS

/// Time maximum to transmit data for SPI
#define SPI_NFC_TIMEOUT_TRANSMISSION	1000

/// Time maximum to wait reception data for SPI
#define SPI_NFC_TIMEOUT_RECEPTION		1000

/**
 * @brief Initialize handler spi used to communicate
 * with module NFC.
 */
void handler_spi_init(SPI_HandleTypeDef *hspi);

/**
 * @brief Read one byte for SPI peripheral
 *
 * @return Return bytes read.
 */
uint8_t spi_get_byte(void);

/**
 * @brief Send one bytes for SPI
 *
 * @param[in]	bytes	Bytes to send.
 */
void spi_send_byte(uint8_t byte);

/**
 * @brief Control Chip select of peripheral.
 *
 * @param[in]	state	Enable/Disable peripheral (true/false).
 */
void spi_set_select(bool state);

/**
 * @brief Get status of pin IRQ on module NFC.
 *
 * @return Return true if was enable or false in otherwise.
 */
bool spi_get_irq(void);

#endif /* INC_SPI_H_ */
