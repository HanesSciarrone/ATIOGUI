/*
 * UART.c
 *
 *  Created on: 30 ago. 2020
 *      Author: Hanes
 */
#include "UART.h"

#ifdef WIFI_UART_RTOS
#include "cmsis_os.h"
#endif

/* Private macros ------------------------------------------------------------ */
#define UART_BUFFER_SIZE                         (1024*2)

/* Private types ------------------------------------------------------------- */
typedef struct
{
  uint8_t  data[UART_BUFFER_SIZE];
  uint16_t index;
  uint16_t position;
}RxBuffer_t;

#ifdef WIFI_UART_RTOS
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
#endif

/* Private variable ---------------------------------------------------------- */
static RxBuffer_t bufferWifi;
static UART_HandleTypeDef *uartWifi;

#ifdef WIFI_UART_RTOS
/* Definitions for wifi_Sem_ReceptionData */
osSemaphoreId_t wifi_Sem_ReceptionDataHandle;
osStaticSemaphoreDef_t Wifi_ReceptionData_ControlBlock;
const osSemaphoreAttr_t wifi_Sem_ReceptionData_attributes = {
  .name = "wifi_Sem_ReceptionData",
  .cb_mem = &Wifi_ReceptionData_ControlBlock,
  .cb_size = sizeof(Wifi_ReceptionData_ControlBlock),
};
#endif


/* Global function implementation -------------------------------------------- */
void WifiUART_SetHandlerUart(UART_HandleTypeDef *uart)
{
	uartWifi = uart;
}

bool_t WifiUART_Operation_Init(void)
{
#ifdef WIFI_UART_RTOS
	/* Semaphore counting initialize blocked because second parameter is 0 */
	wifi_Sem_ReceptionDataHandle = osSemaphoreNew(2048, 0, &wifi_Sem_ReceptionData_attributes);

	if (wifi_Sem_ReceptionDataHandle == NULL)
	{
		return FALSE;
	}
#endif

	/* Once the WiFi UART is intialized, start an asynchrounous recursive
	   listening. the HAL_UART_Receive_IT() call below will wait until one char is
	   received to trigger the HAL_UART_RxCpltCallback(). The latter will recursively
	   call the former to read another char.  */
	bufferWifi.index = 0;
	bufferWifi.position = 0;

	HAL_UART_Receive_IT(uartWifi, (uint8_t *)&bufferWifi.data[bufferWifi.index], 1);

	return TRUE;
}

int8_t WifiUART_Send(const uint8_t* data, uint32_t length)
{
#ifdef WIFI_UART_RTOS
	if (HAL_UART_Transmit_IT(uartWifi, (uint8_t*)data, length) != HAL_OK)
	{
		return -1;
	}
#else
	if (HAL_UART_Transmit(uartWifi, (uint8_t*)data, length, DEFAULT_TIME_OUT) != HAL_OK)
	{
		return -1;
	}
#endif
	return 0;
}

int32_t WifiUART_Receive(uint8_t* buffer, uint32_t length)
{
	uint32_t readData = 0;

	/* Loop until data received */
	#ifdef WIFI_UART_RTOS
	osStatus_t status;

	while (length--)
	{
		status = osSemaphoreAcquire(wifi_Sem_ReceptionDataHandle, DEFAULT_TIME_OUT/portTICK_PERIOD_MS);
		if (status != osOK )
		{
			break;
		}

		if(bufferWifi.position != bufferWifi.index)
		{
			/* serial data available, so return data to user */
			*buffer++ = bufferWifi.data[bufferWifi.position++];
			readData++;

			/* check for ring buffer wrap */
			if (bufferWifi.position >= UART_BUFFER_SIZE)
			{
				/* Ring buffer wrap, so reset head pointer to start of buffer */
				bufferWifi.position = 0;
			}
		}
	}
#else
	uint32_t tickStart;

	while (length--)
	{
		tickStart = HAL_GetTick();
		do
		{
			if(bufferWifi.position != bufferWifi.index)
			{
				/* serial data available, so return data to user */
				*buffer++ = bufferWifi.data[bufferWifi.position++];
				readData++;

				/* check for ring buffer wrap */
				if (bufferWifi.position >= UART_BUFFER_SIZE)
				{
					/* Ring buffer wrap, so reset head pointer to start of buffer */
					bufferWifi.position = 0;
				}
				break;
			}
		}while((HAL_GetTick() - tickStart ) < DEFAULT_TIME_OUT);
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
	if(++bufferWifi.index >= UART_BUFFER_SIZE)
	{
		bufferWifi.index = 0;
	}

	HAL_UART_Receive_IT(uartHandle, (uint8_t *)&bufferWifi.data[bufferWifi.index], 1);

	/* Internally this function call xSemaphoreGiveFromISR if
	 * function is calling inside of interruption.
	 */
	osSemaphoreRelease(wifi_Sem_ReceptionDataHandle);
}







