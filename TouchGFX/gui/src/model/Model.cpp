#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

/* Include used to model for communicative with backend */
#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"
#include "ModuleWifi.h"

#define MAXBUFFER_LITERS	20
#define BUFFER_SIZE_USER_ID	20

// Global variable declared on ModuleWifi.c
extern osMessageQueueId_t queue_Wifi_operationHandle;
extern osSemaphoreId_t semaphore_new_msg_nfc;
extern osMutexId_t mutex_NewMsg_WifiHandle;
extern WifiMessage_t wifiParameters;
extern uint32_t keep_alive_connection;
extern uint8_t version_mqtt;
extern uint8_t qos_mqtt;
extern uint8_t	client_id[BUFFER_SIZE_TOPIC];
extern uint8_t publish_topic[BUFFER_SIZE_TOPIC];
extern uint8_t suscribe_topic[BUFFER_SIZE_TOPIC];
extern uint8_t user_id[BUFFER_SIZE_USER_ID], length_id;

/* --------------------- Global variable --------------------- */
osMessageQueueId_t	queue_NewMsg_GUI;
const osMessageQueueAttr_t queue_GUI_attributes = {
  .name = "wifiqueue_operation"
};

static uint32_t liters_available;
static gui_network_t list_network;

Model::Model() : modelListener(0)
{
	queue_NewMsg_GUI = osMessageQueueNew(2, sizeof(uint8_t), &queue_GUI_attributes);
}

void Model::tick()
{
	uint8_t i;
	uint8_t message;

	if (osMessageQueueGet(queue_NewMsg_GUI, &message, 0L, 0) == osOK)
	{
		switch(message)
		{
			// Resulting operation of scan network
			case 0:
			{
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
				memset(&list_network, 0, sizeof(gui_network_t));
				for(i = 0; i < 30; i++) {
					strncpy((char *)list_network.listNetwork[i].ssid, (char *)wifiParameters.listNetwork[i].ssid, strlen((char *)wifiParameters.listNetwork[i].ssid));
					list_network.listNetwork[i].rssi = wifiParameters.listNetwork[i].rssi;
				}
				modelListener->ResultScandNetwork(&list_network);
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

			// Resulting operation of send credential
			case 2:{
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
				if(wifiParameters.resultOperation == VALID_USER) {
					liters_available = atoi((char *)wifiParameters.data);
				}
				else {
					liters_available = 0;
				}

				modelListener->show_status_credential(wifiParameters.resultOperation);

				osMutexRelease(mutex_NewMsg_WifiHandle);
			}

			// Show progress bar
			case 3: {
				modelListener->ShowProgreessBar();
			}
			break;

			case 4: {
				modelListener->getting_data_read_card(user_id, length_id);
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

	memset(wifiParameters.data, 0, MAX_LENGTH_MESSAGE_CREDENTIAL*sizeof(uint8_t));
	wifiParameters.data[0] = EVENT_SEND_CREDENTIAL;
	wifiParameters.data[1] = '|';
	strncat((char *)wifiParameters.data, (char *)buffer, length);

	osMutexRelease(mutex_NewMsg_WifiHandle);

	msg = SEND_CREDENTIAL;
	if (osMessageQueuePut(queue_Wifi_operationHandle, &msg, 0L, 0) == osOK) {
		modelListener->ShowProgreessBar();
	}
}

uint32_t Model::get_liters_fuel_available(void)
{
	return liters_available;
}

void Model::configure_parameters_mqtt(struct parameters_mqtt_s param)
{
	osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
	keep_alive_connection = param.keep_alive;
	version_mqtt = param.keep_alive;
	qos_mqtt = param.qos;
	strncpy((char *)client_id, (char *)param.client_id, strlen((char *)param.client_id));
	strncpy((char *)publish_topic, (char *)param.publish_topic, strlen((char *)param.publish_topic));
	strncpy((char *)suscribe_topic, (char *)param.suscribe_topic, strlen((char *)param.suscribe_topic));
	osMutexRelease(mutex_NewMsg_WifiHandle);
}

void Model::active_reader(void)
{
	osSemaphoreRelease(semaphore_new_msg_nfc);
}
