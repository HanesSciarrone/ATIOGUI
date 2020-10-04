#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

/* Include used to model for communicative with backend */
#include <string.h>

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
			// Resulting operation of scan network
			case 0:
			{
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
				modelListener->ResultScandNetwork(&wifiParameters);
				osMutexRelease(mutex_NewMsg_WifiHandle);
			}
			break;

			// Resulting operation of connect to network
			case 1:
			{
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
				modelListener->ShowStatusConnectionNetwork(wifiParameters.resultOperation);
				osMutexRelease(mutex_NewMsg_WifiHandle);
			}
		}
	}
}

void Model::MsgScanNetwork()
{
	WifiModule_Operation msg;
	osStatus_t status;

	msg = SCAN_NETWORK;
	status = osMessageQueuePut(queue_Wifi_operationHandle, &msg, 0L, 0);

	if (status == osOK)
	{
		modelListener->ShowProgreessBar();
	}
}

void Model::MsgConnectNetwork(char *ssid, char *password)
{
	WifiModule_Operation msg;
	osStatus_t status;

	osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);

	memset(wifiParameters.ssid, 0, MAX_LENGTH_CONNECTION_NETWORK);
	strncpy((char *)wifiParameters.ssid, ssid, strlen(ssid));

	memset(wifiParameters.password, 0, MAX_LENGTH_CONNECTION_NETWORK);
	strncpy((char *)wifiParameters.password, password, strlen(password));

	osMutexRelease(mutex_NewMsg_WifiHandle);

	msg = CONNECT_NETWORK;
	status = osMessageQueuePut(queue_Wifi_operationHandle, &msg, 0L, 0);

	if (status == osOK)
	{
		modelListener->ShowProgreessBar();
	}
}

void Model::sent_credential_to_IoT(uint8_t *buffer, uint16_t length)
{
	WifiModule_Operation msg;

	osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);

	memset(wifiParameters.credential, 0, MAX_LENGTH_CREDENTIAL);
	strncpy((char *)wifiParameters.credential, (char *)buffer, strlen((char *)buffer));

	osMutexRelease(mutex_NewMsg_WifiHandle);

	msg = SEND_PACKET;
	if (osMessageQueuePut(queue_Wifi_operationHandle, &msg, 0L, 0) == osOK) {
		modelListener->ShowProgreessBar();
	}
}
