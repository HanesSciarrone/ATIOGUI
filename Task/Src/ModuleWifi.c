/*
 * ModuleWifi.c
 *
 *  Created on: Aug 29, 2020
 *      Author: Hanes
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cmsis_os.h"
#include "ModuleWifi.h"
#include "UART.h"

/* Include of module driver */
#include "ESP8266.h"

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;

/* Definitions for TaskWifi */
osThreadId_t TaskWifiHandle;
const osThreadAttr_t TaskWifi_attributes = {
  .name = "TaskWifi",
  .priority = (osPriority_t) osPriorityNormal2,
  .stack_size = 1024 * 4
};

/* Definitions for mutex_NewMsg_Wifi */
osMutexId_t mutex_NewMsg_WifiHandle;
osStaticMutexDef_t mutex_NewMsg_WifiControlBlock;
const osMutexAttr_t mutex_NewMsg_Wifi_attributes = {
  .name = "mutex_NewMsg_Wifi",
  .cb_mem = &mutex_NewMsg_WifiControlBlock,
  .cb_size = sizeof(mutex_NewMsg_WifiControlBlock),
};

/* Definitions for sem_Wifi_OpComplete */
osSemaphoreId_t sem_Wifi_OpCompleteHandle;
osStaticSemaphoreDef_t sem_Wifi_OpCompletControlBlock;
const osSemaphoreAttr_t sem_Wifi_OpComplete_attributes = {
  .name = "sem_Wifi_OpComplete",
  .cb_mem = &sem_Wifi_OpCompletControlBlock,
  .cb_size = sizeof(sem_Wifi_OpCompletControlBlock),
};

/* Private variable ----------------------------------------------------------*/
static ESP8266_CommInterface_s commInterface;
static WifiModule_Operation operation;	// Variable used to indicate to task what must do
static ListNetwork_t listNetwork[MAX_COUNT_LIST];

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief All functionality of task that control Module Wi-fi is here
 *
 * @param[in] argument Pointer void used to pass parameters of task
 */
static void ModuleWifi(void *argument);

/**
 * @brief Function to initialize communication interface of
 * library ESP8266
 *
 * @retval Return 1 if was successor 0 in  oher way.
 */
static bool_t WifiModule_Comm_Init(void);

/**
 * @brief Initialize module ESP8266 before start to work
 *
 * @retval Return 1 was success or 0 in other way.
 */
static bool_t WifiModule_Init(void);

/**
 * @brief Function used to configure information will give Wi-Fi module
 * about nearby network and search that.
 *
 * @param[in, out] Structure with all data saved
 *
 * @return Return 1 if operation is success or 0 in otherwise
 */
static bool_t ModuleWifi_ScanNetwork(void);




/* Private function implementation --------------------------------------------*/
static bool_t ModuleWifi_ScanNetwork(void)
{
	uint8_t buffer[MAX_BUFFER_SIZE], *ptrStart, *ptrEnd;
	uint8_t lvl_signal[4];
	uint8_t index = 0;

	memset(listNetwork, 0, MAX_COUNT_LIST*sizeof(ListNetwork_t));
	if (ESP8266_SetListNeetwork() != ESP8266_OK)
	{
		return FALSE;
	}

	if (ESP8266_ScanNetwork() != ESP8266_OK)
	{
		return FALSE;
	}

	ESP8266_GetModuleResponse(buffer, MAX_BUFFER_SIZE);
	ptrStart = buffer;

	while((ptrStart = (uint8_t *)strstr((const char *)ptrStart, "+CWLAP:")) != NULL)
	{
		ptrStart += 9;	// Start of network name
		ptrEnd = (uint8_t *)strstr((const char *)ptrStart, "\",-"); //Looking for network name end

		// If string is cut so exit of loop
		if (ptrEnd == NULL)
		{
			break;
		}
		// Ask of larger than buffer
		if (ptrEnd-ptrStart < 21)
		{
			strncpy((char *)listNetwork[index].ssid, (char *)ptrStart, ptrEnd-ptrStart);
		}
		else
		{
			strncpy((char *)listNetwork[index].ssid, (char *)ptrStart, 20);
		}
		listNetwork[index].ssid[21] = '\0';

		//Move pointer to end of name
		ptrStart = ptrEnd;
		ptrEnd = (uint8_t *)strstr((const char *)ptrStart, ")\r\n"); //Looking for network name end
		if (ptrEnd == NULL)
		{
			break;
		}

		// Move pointer to divisor between name and signal strength
		ptrStart += 2;
		memset(lvl_signal, 0, 4);
		strncpy((char *)lvl_signal, (char *)ptrStart, 3);
		listNetwork[index].rssi = atoi((char *)lvl_signal);

		index++;
	}

	return TRUE;
}

static bool_t WifiModule_Init(void)
{
	// Configure module for avoid echo
	if ( ESP8266_SetEcho(0) != ESP8266_OK)
	{
		return 0;
	}

	// Configure module as Station Mode
	if ( ESP8266_SetModeWIFI((uint8_t *)"1") != ESP8266_OK)
	{
		return 0;
	}

	// Configure module as Station Mode
	if ( ESP8266_SetMultipleConnection((uint8_t *)"0") != ESP8266_OK)
	{
		return 0;
	}

	return 1;
}

static bool_t WifiModule_Comm_Init(void)
{
	commInterface.send = &WifiUART_Send;
	commInterface.recv = &WifiUART_Receive;

	return (ESP8266_CommInterface_Init(&commInterface) == ESP8266_OK) ? TRUE : FALSE;
}

static void ModuleWifi(void *argument)
{
	bool_t result_Operation = TRUE;
	/* Initialization of library ESP8266 */
	if (!WifiModule_Comm_Init())
	{
		osSemaphoreDelete(sem_Wifi_OpCompleteHandle);
		osMutexDelete(mutex_NewMsg_WifiHandle);
		osThreadTerminate(TaskWifiHandle);
	}

	/* Initialization of module ESP8266 */
	if (!WifiModule_Init())
	{
		osSemaphoreDelete(sem_Wifi_OpCompleteHandle);
		osMutexDelete(mutex_NewMsg_WifiHandle);
		osThreadTerminate(TaskWifiHandle);
	}

	operation = SCAN_NETWORK;

	while(1)
	{
		switch(operation)
		{
			case SCAN_NETWORK:
			{
				result_Operation = ModuleWifi_ScanNetwork();
			}
			break;

			default:
			{
				result_Operation = FALSE;
			}
		}

		osSemaphoreRelease(sem_Wifi_OpCompleteHandle);

		osDelay(1000/portTICK_PERIOD_MS);
	}
	/* USER CODE END ModuleWifi */
}

/* Public function implementation --------------------------------------------*/
bool_t ModuleWifi_Started(void)
{
	/* Create the mutex(es) */
	mutex_NewMsg_WifiHandle = osMutexNew(&mutex_NewMsg_Wifi_attributes);
	if (mutex_NewMsg_WifiHandle == NULL)
	{
		return FALSE;
	}

	/* Create the semaphores(s) */
	sem_Wifi_OpCompleteHandle = osSemaphoreNew(1, 0, &sem_Wifi_OpComplete_attributes); // Semaphore initialize blocked
	if (sem_Wifi_OpCompleteHandle == NULL)
	{
		return FALSE;
	}

	/* Create the thread */
	TaskWifiHandle = osThreadNew(ModuleWifi, NULL, &TaskWifi_attributes);
	if (TaskWifiHandle == NULL)
	{
		return FALSE;
	}

	return TRUE;
}
