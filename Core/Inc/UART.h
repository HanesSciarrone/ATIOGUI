/*
 * uart.h
 *
 *  Created on: 30 ago. 2020
 *      Author: Hanes
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"

/// Macro defined if you use RTOS, if not comment macros
#define WIFI_UART_RTOS

/// Macro defines timeout on reception operation
#define DEFAULT_TIME_OUT	3000

/**
 * @brief Setup uart handler used to drive wifi module
 *
 * @param[in] uart Pointer of uart handler used.
 */
void WifiUART_SetHandlerUart(UART_HandleTypeDef *uart);

/**
 * @brief Initialize interruption callback and synchronize if you use
 * RTOS operation system on your firmware
 *
 * @return Return 1 if operation was success or 0 in otherwise
 */
bool_t WifiUART_Operation_Init(void);

/**
  * @brief  Send Data to the Wi-Fi module over the UART interface.
  *
  * @param[in]	data 	Data that you would like to sent.
  * @param[in]	length	Length of data at sent
  *
  * @return Return 0 if operation was success or -1 in otherwise.
  */
int8_t WifiUART_Send(const uint8_t* data, uint32_t length);

/**
  * @brief  Receive Data from the Wi-Fi module over the UART interface.
  *
  * @param[in,out]  buffer 	Buffer used to storage data read.
  * @param[in]  	length 	Maximum size of the data to receive.
  *
  * @return The actual data size that has been received.
  */
int32_t WifiUART_Receive(uint8_t* buffer, uint32_t length);

#endif /* INC_UART_H_ */
