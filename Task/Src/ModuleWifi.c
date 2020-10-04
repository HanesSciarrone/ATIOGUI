/*
 * ModuleWifi.c
 *
 *  Created on: Aug 29, 2020
 *      Author: Hanes
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cmsis_os.h"
#include "ModuleWifi.h"
#include "UART.h"

/* Include of module driver */
#include "ESP8266.h"

/* Handler of GUI for sent message ------------------------------------------ */
extern osMessageQueueId_t	queue_NewMsg_GUI;

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;

/* Definition of task, queue, semaphore and mutex handler */

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

/* Definitions for wifiqueue_operation */
osMessageQueueId_t queue_Wifi_operationHandle;
const osMessageQueueAttr_t wifiqueue_operation_attributes = {
  .name = "wifiqueue_operation"
};


/* Private variable ----------------------------------------------------------*/
static ESP8266_CommInterface_s commInterface;
WifiMessage_t wifiParameters;

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
 * @return Return 1 if operation is success or 0 in otherwise
 */
static bool_t ModuleWifi_ScanNetwork(void);




/* Private function implementation --------------------------------------------*/
static bool_t ModuleWifi_ScanNetwork(void)
{
	uint8_t buffer[MAX_BUFFER_SIZE], *ptrStart = NULL, *ptrEnd = NULL;
	uint8_t lvl_signal[4];
	uint8_t index = 0;

	memset(wifiParameters.listNetwork, 0, MAX_COUNT_LIST*sizeof(ListNetwork_t));
	if (ESP8266_SetListNeetwork() != ESP8266_OK)
	{
		return 0;
	}

	if (ESP8266_ScanNetwork() != ESP8266_OK)
	{
		return 0;
	}

	ESP8266_GetModuleResponse(buffer, MAX_BUFFER_SIZE);
	ptrStart = buffer;

	while((ptrStart = (uint8_t *)strstr((const char *)ptrStart, "+CWLAP:")) != NULL)
	{
		if (index >= MAX_COUNT_LIST)
		{
			break;
		}

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
			strncpy((char *)wifiParameters.listNetwork[index].ssid, (char *)ptrStart, ptrEnd-ptrStart);
		}
		else
		{
			strncpy((char *)wifiParameters.listNetwork[index].ssid, (char *)ptrStart, 20);
		}
		wifiParameters.listNetwork[index].ssid[21] = '\0';

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
		wifiParameters.listNetwork[index].rssi = atoi((char *)lvl_signal);

		index++;
	}

	return 1;
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

	return (ESP8266_CommInterface_Init(&commInterface) == ESP8266_OK) ? 1 : 0;
}

static void ModuleWifi(void *argument)
{
	uint8_t msgGUI;
	WifiModule_Operation operation;

	/* Initialization of library ESP8266 */
	if (!WifiModule_Comm_Init())
	{
		osMutexDelete(mutex_NewMsg_WifiHandle);
		osThreadTerminate(TaskWifiHandle);
	}

	/* Initialization of module ESP8266 */
	if (!WifiModule_Init())
	{
		osMutexDelete(mutex_NewMsg_WifiHandle);
		osThreadTerminate(TaskWifiHandle);
	}

	operation = SCAN_NETWORK;

	while(1)
	{
		osMessageQueueGet(queue_Wifi_operationHandle,&operation, 0L, osWaitForever);

		switch(operation)
		{
			case SCAN_NETWORK:
			{
				ModuleWifi_ScanNetwork();
				msgGUI = 0;
				osMessageQueuePut(queue_NewMsg_GUI, &msgGUI, 0L, osWaitForever);
			}
			break;

			case CONNECT_NETWORK:
			{
				ESP8266_NetworkParameters_s network;
				ESP8266_StatusTypeDef_t status;

				status = ESP8266_DisconnectAllNetwork();
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);

				network.ssid = wifiParameters.ssid;
				network.password = wifiParameters.password;
				status = ESP8266_ConnectionNetwork(&network);

				if (status == ESP8266_OK)
				{
					wifiParameters.resultOperation = 1;
				}
				else
				{
					wifiParameters.resultOperation = 0;
				}

				osMutexRelease(mutex_NewMsg_WifiHandle);

				msgGUI = 1;
				osMessageQueuePut(queue_NewMsg_GUI, &msgGUI, 0L, osWaitForever);
			}
			break;

			case SEND_PACKET:
			{

			}
			break;

		}
	}
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

	 /* creation of wifiqueue_operation */
	queue_Wifi_operationHandle = osMessageQueueNew (2, sizeof(WifiModule_Operation), &wifiqueue_operation_attributes);
	if (queue_Wifi_operationHandle == NULL)
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


bool_t ModuleWifi_MsgScanNetwork(void)
{
	WifiModule_Operation msg;
	osStatus_t status;

	msg = SCAN_NETWORK;
	status = osMessageQueuePut(queue_Wifi_operationHandle, &msg, 0L, 0);

	return (status == osOK) ? TRUE : FALSE;
}
