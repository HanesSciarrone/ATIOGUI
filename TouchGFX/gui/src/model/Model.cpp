#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "touchgfx/Unicode.hpp"

/* Include used to model for communicative with backend */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"
#include "ModuleWifi.h"

#define MAXBUFFER_LITERS	20
#define BUFFER_SIZE_USER_ID	20

// Global variable declared on ModuleWifi.c
extern osMessageQueueId_t queue_wifi_operation_handle;
extern osSemaphoreId_t semaphore_new_msg_nfc;
extern osMutexId_t mutex_new_msg_wifi_handle;
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

static float liters_available;		/// Liters available for user ID
static gui_network_t list_network;
static uint8_t liters_dispache[20];	/// Liters to dispache entried for user.

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
				osMutexAcquire(mutex_new_msg_wifi_handle, osWaitForever);
				memset(&list_network, 0, sizeof(gui_network_t));
				for(i = 0; i < 30; i++) {
					strncpy((char *)list_network.listNetwork[i].ssid, (char *)wifiParameters.listNetwork[i].ssid, strlen((char *)wifiParameters.listNetwork[i].ssid));
					list_network.listNetwork[i].rssi = wifiParameters.listNetwork[i].rssi;
				}
				modelListener->ResultScandNetwork(&list_network);
				osMutexRelease(mutex_new_msg_wifi_handle);
			}
			break;

			// Resulting operation of connect to network
			case 1:
			{
				osMutexAcquire(mutex_new_msg_wifi_handle, osWaitForever);
				modelListener->ShowStatusConnectionNetwork(wifiParameters.resultOperation);
				osMutexRelease(mutex_new_msg_wifi_handle);
			}

			// Resulting operation of send credential
			case 2:{
				osMutexAcquire(mutex_new_msg_wifi_handle, osWaitForever);
				if(wifiParameters.resultOperation == VALID_USER) {
					liters_available = atof((char *)wifiParameters.data);
				}
				else {
					liters_available = 0;
				}

				modelListener->show_status_credential(wifiParameters.resultOperation);

				osMutexRelease(mutex_new_msg_wifi_handle);
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

	msg = SCAN_NETWORK;
	if (osMessageQueuePut(queue_wifi_operation_handle, &msg, 0L, 0) != osOK) {
		modelListener->hide_progreess_bar();
	}
}

void Model::MsgConnectNetwork(char *ssid, char *password)
{
	WifiModule_Operation msg;

	osMutexAcquire(mutex_new_msg_wifi_handle, osWaitForever);

	memset(wifiParameters.ssid, 0, MAX_LENGTH_CONNECTION_NETWORK);
	strncpy((char *)wifiParameters.ssid, ssid, strlen(ssid));

	memset(wifiParameters.password, 0, MAX_LENGTH_CONNECTION_NETWORK);
	strncpy((char *)wifiParameters.password, password, strlen(password));

	osMutexRelease(mutex_new_msg_wifi_handle);

	msg = CONNECT_NETWORK;
	if (osMessageQueuePut(queue_wifi_operation_handle, &msg, 0L, 0) != osOK) {
		modelListener->hide_progreess_bar();
	}
}

void Model::sent_credential_to_IoT(uint8_t *buffer, uint16_t length)
{
	WifiModule_Operation msg;

	osMutexAcquire(mutex_new_msg_wifi_handle, osWaitForever);

	memset(wifiParameters.data, 0, MAX_LENGTH_MESSAGE_CREDENTIAL*sizeof(uint8_t));
	wifiParameters.data[0] = EVENT_SEND_CREDENTIAL;
	wifiParameters.data[1] = '|';
	strncat((char *)wifiParameters.data, (char *)buffer, length);

	osMutexRelease(mutex_new_msg_wifi_handle);

	msg = SEND_CREDENTIAL;
	if (osMessageQueuePut(queue_wifi_operation_handle, &msg, 0L, 0) != osOK) {
		modelListener->hide_progreess_bar();
	}
}

float Model::get_liters_fuel_available(void)
{
	return liters_available;
}

void Model::set_liters_to_dispache(uint8_t *liters_selected)
{
	memset(liters_dispache, 0, sizeof(liters_dispache));
	strncpy((char *)liters_dispache, (char *)liters_selected, strlen((char *)liters_selected));
}

uint8_t *Model::get_liters_to_dispahe()
{
	return liters_dispache;
}

void Model::configure_parameters_mqtt(struct parameters_mqtt_s param)
{
	osMutexAcquire(mutex_new_msg_wifi_handle, osWaitForever);
	keep_alive_connection = param.keep_alive;
	version_mqtt = param.version;
	qos_mqtt = param.qos;
	memset(client_id, 0, BUFFER_SIZE_TOPIC);
	memset(publish_topic, 0, BUFFER_SIZE_TOPIC);
	memset(suscribe_topic, 0, BUFFER_SIZE_TOPIC);
	strncpy((char *)client_id, (char *)param.client_id, strlen((char *)param.client_id));
	strncpy((char *)publish_topic, (char *)param.publish_topic, strlen((char *)param.publish_topic));
	strncpy((char *)suscribe_topic, (char *)param.suscribe_topic, strlen((char *)param.suscribe_topic));
	osMutexRelease(mutex_new_msg_wifi_handle);
}

void Model::active_reader(void)
{
	osSemaphoreRelease(semaphore_new_msg_nfc);
}
