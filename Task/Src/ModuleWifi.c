/*
 * ModuleWifi.c
 *
 *  Created on: Aug 29, 2020
 *      Author: Hanes
 */

#include <stddef.h>

#include "cmsis_os.h"
#include "ModuleWifi.h"

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




/* Private function implementation --------------------------------------------*/
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
	//commInterface.send = &WIFI_UART_Send;
	//commInterface.recv = &WIFI_UART_Receive;

	return (ESP8266_CommInterface_Init(&commInterface) == ESP8266_OK) ? TRUE : FALSE;
}

static void ModuleWifi(void *argument)
{
	ESP8266_StatusTypeDef_t status;


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

	while(1)
	{
		osDelay(1/portTICK_PERIOD_MS);
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
