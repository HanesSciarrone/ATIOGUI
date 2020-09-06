#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

/* Include used to model for communicative with backend */
#include "cmsis_os.h"
#include "ModuleWifi.h"

// Global variable declared on ModuleWifi.c
extern osMessageQueueId_t queue_Wifi_operationHandle;
extern osMutexId_t mutex_NewMsg_WifiHandle;
extern WifiMessage_t wifiParameters;

/* --------------------- Global variable --------------------- */
osMessageQueueId_t	queue_NewMsg_GUI;
const osMessageQueueAttr_t queue_GUI_attributes = {
  .name = "wifiqueue_operation"
};

Model::Model() : modelListener(0)
{
	queue_NewMsg_GUI = osMessageQueueNew(2, sizeof(uint8_t), &queue_GUI_attributes);
}

void Model::tick()
{
	uint8_t message;

	if (osMessageQueueGet(queue_NewMsg_GUI, &message, 0L, 0) == osOK)
	{
		switch(message)
		{
			case 0:
			{
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
				modelListener->ResultScandNetwork(&wifiParameters);
				osMutexRelease(mutex_NewMsg_WifiHandle);
			}
			break;
		}
	}
}

void Model::MsgScanNetwork()
{
	WifiModule_Operation msg;
	osStatus_t status;

	msg = SCAN_NETWORK;
	status = osMessageQueuePut(queue_Wifi_operationHandle, &msg, 0L, 0);
}
