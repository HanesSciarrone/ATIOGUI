/*
 * UART.c
 *
 *  Created on: 30 ago. 2020
 *      Author: Hanes
 */
#include <string.h>

#include "UART.h"

#ifdef UART_RTOS
#include "cmsis_os.h"
#endif

/* Private macros ------------------------------------------------------------ */
#define WIFI_UART_BUFFER_SIZE			(1024*2)
#define PUMP_CONTROLLER_BUFFER_SIZE		1024

/* Private types ------------------------------------------------------------- */
typedef struct
{
  uint8_t  data[WIFI_UART_BUFFER_SIZE];
  uint16_t index;
  uint16_t position;
}wifi_rx_buffer_t;

typedef struct
{
  uint8_t  data[PUMP_CONTROLLER_BUFFER_SIZE];
  uint16_t index;
  uint16_t position;
}pump_controller_rx_buffer_t;

#ifdef UART_RTOS
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
#endif

/* Private variable ---------------------------------------------------------- */
static wifi_rx_buffer_t buffer_wifi;
static pump_controller_rx_buffer_t	buffer_pump_controller;
static UART_HandleTypeDef *uart_wifi;
static UART_HandleTypeDef *uart_pump_controller;

#ifdef UART_RTOS
/* Definitions for wifi_Sem_ReceptionData */
osSemaphoreId_t wifi_sem_reception_data_handle;
osStaticSemaphoreDef_t wifi_reception_data_control_block;
const osSemaphoreAttr_t wifi_sem_reception_data_attributes = {
  .name = "wifi_sem_reception_data",
  .cb_mem = &wifi_reception_data_control_block,
  .cb_size = sizeof(wifi_reception_data_control_block),
};

/* Definitions for pump_controller_sem_reception_data */
osSemaphoreId_t pump_controller_sem_reception_data_handle;
osStaticSemaphoreDef_t pump_controller_data_control_block;
const osSemaphoreAttr_t pump_controller_sem_reception_data_attributes = {
  .name = "pump_controller_sem_reception_data",
  .cb_mem = &pump_controller_data_control_block,
  .cb_size = sizeof(pump_controller_data_control_block),
};
#endif


/* Global function implementation -------------------------------------------- */
void wifi_uart_set_handler_uart(UART_HandleTypeDef *uart)
{
	uart_wifi = uart;
}

void pump_controller_uart_set_handler_uart(UART_HandleTypeDef *uart)
{
	uart_pump_controller = uart;
}

bool_t wifi_uart_operation_init(void)
{
#ifdef UART_RTOS
	/* Semaphore counting initialize blocked because second parameter is 0 */
	wifi_sem_reception_data_handle = osSemaphoreNew(2048, 0, &wifi_sem_reception_data_attributes);

	if (wifi_sem_reception_data_handle == NULL) {
		return FALSE;
	}
#endif

	/* Once the WiFi UART is intialized, start an asynchrounous recursive
	   listening. the HAL_UART_Receive_IT() call below will wait until one char is
	   received to trigger the HAL_UART_RxCpltCallback(). The latter will recursively
	   call the former to read another char.  */
	buffer_wifi.index = 0;
	buffer_wifi.position = 0;
	strncpy((char *)buffer_wifi.data, "\0", WIFI_UART_BUFFER_SIZE);
	HAL_UART_Receive_IT(uart_wifi, (uint8_t *)&buffer_wifi.data[buffer_wifi.index], 1);
	return TRUE;
}

bool pump_controller_uart_operation_init(void)
{
#ifdef UART_RTOS
	/* Semaphore counting initialize blocked because second parameter is 0 */
	pump_controller_sem_reception_data_handle = osSemaphoreNew(2048, 0, &pump_controller_sem_reception_data_attributes);

	if (pump_controller_sem_reception_data_handle == NULL) {
		return false;
	}
#endif

	/* Once the WiFi UART is intialized, start an asynchrounous recursive
	   listening. the HAL_UART_Receive_IT() call below will wait until one char is
	   received to trigger the HAL_UART_RxCpltCallback(). The latter will recursively
	   call the former to read another char.  */
	buffer_pump_controller.index = 0;
	buffer_pump_controller.position = 0;
	strncpy((char *)buffer_pump_controller.data, "\0", PUMP_CONTROLLER_BUFFER_SIZE);
	HAL_UART_Receive_IT(uart_pump_controller, (uint8_t *)&buffer_pump_controller.data[buffer_pump_controller.index], 1);
	return true;
}

int8_t wifi_uart_sent(const uint8_t* data, uint32_t length)
{
#ifdef UART_RTOS
	if (HAL_UART_Transmit_IT(uart_wifi, (uint8_t*)data, length) != HAL_OK) {
		return -1;
	}
#else
	if (HAL_UART_Transmit(uart_wifi, (uint8_t*)data, length, DEFAULT_TIME_OUT) != HAL_OK) {
		return -1;
	}
#endif
	return 0;
}

int32_t wifi_uart_receive(uint8_t* buffer, uint32_t length, uint32_t timeout)
{
	uint32_t readData = 0;

	/* Loop until data received */
	#ifdef UART_RTOS
	osStatus_t status;

	while (length--) {
		status = osSemaphoreAcquire(wifi_sem_reception_data_handle, timeout/portTICK_PERIOD_MS);
		if (status != osOK ) {
			break;
		}

		if(buffer_wifi.position != buffer_wifi.index) {
			/* serial data available, so return data to user */
			*buffer++ = buffer_wifi.data[buffer_wifi.position++];
			readData++;

			/* check for ring buffer wrap */
			if (buffer_wifi.position >= WIFI_UART_BUFFER_SIZE) {
				/* Ring buffer wrap, so reset head pointer to start of buffer */
				buffer_wifi.position = 0;
			}
		}
	}
#else
	uint32_t tickStart;

	while (length--) {
		tickStart = HAL_GetTick();
		do {
			if(buffer_wifi.position != buffer_wifi.index) {
				/* serial data available, so return data to user */
				*buffer++ = buffer_wifi.data[buffer_wifi.position++];
				readData++;

				/* check for ring buffer wrap */
				if (buffer_wifi.position >= WIFI_UART_BUFFER_SIZE) {
					/* Ring buffer wrap, so reset head pointer to start of buffer */
					buffer_wifi.position = 0;
				}
				break;
			}
		} while((HAL_GetTick() - tickStart ) < DEFAULT_TIME_OUT);
	}
	#endif


	return readData;
}

/**
  * @brief  Rx Callback when new data is received on the UART.
  * @param  UartHandle: Uart handle receiving the data.
  * @return None.
  */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *uartHandle)
{
	/* If ring buffer end is reached reset tail pointer to start of buffer */
	if (uartHandle == uart_wifi) {
		if(++buffer_wifi.index >= WIFI_UART_BUFFER_SIZE) {
			buffer_wifi.index = 0;
		}

		HAL_UART_Receive_IT(uartHandle, (uint8_t *)&buffer_wifi.data[buffer_wifi.index], 1);

		/* Internally this function call xSemaphoreGiveFromISR if
		 * function is calling inside of interruption.
		 */
#ifdef UART_RTOS
		osSemaphoreRelease(wifi_sem_reception_data_handle);
#endif
	}
}







