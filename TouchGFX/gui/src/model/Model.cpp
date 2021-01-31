#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "touchgfx/Unicode.hpp"

/* Include used to model for communicative with backend */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"
#include "ModuleWifi.h"
#include "ModuleControllerPump.h"

#define SIZE_BUFFER_LITERS	20
#define BUFFER_SIZE_USER_ID	20

// Variable used for Wifi module
extern osMessageQueueId_t queue_wifi_operation_handle;
extern osMutexId_t mutex_new_msg_wifi_handle;
extern WifiMessage_t wifiParameters;
extern uint32_t keep_alive_connection;
extern uint8_t version_mqtt;
extern uint8_t qos_mqtt;
extern uint8_t	client_id[BUFFER_SIZE_TOPIC];
extern uint8_t publish_topic[BUFFER_SIZE_TOPIC];
extern uint8_t suscribe_topic[BUFFER_SIZE_TOPIC];

//Variable used for NFC reader
extern osSemaphoreId_t semaphore_new_msg_nfc;
extern uint8_t user_id[BUFFER_SIZE_USER_ID], length_id;

// Variable used for pump controller
extern osMutexId_t mutex_new_msg_pump_controller_handle;
extern osMessageQueueId_t controller_pump_queueHandle;
extern uint8_t number_pump;
extern uint8_t liters_fuel[20];
extern enum type_fuel_t type_fuel;

/* --------------------- Global variable --------------------- */
osMessageQueueId_t	queue_NewMsg_GUI;
const osMessageQueueAttr_t queue_GUI_attributes = {
  .name = "wifiqueue_operation"
};

static gui_network_t list_network;
static float liters_available;						/// Liters available for user ID
static uint8_t liters_dispache[SIZE_BUFFER_LITERS];	/// Liters to dispache entried for user.

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

uint8_t *get_user_id(void)
{
	return user_id;
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

void Model::dispatch_fuel_action(uint8_t *pump, uint8_t * type)
{
	uint8_t msg = DISPACHE_PUMP;

	osMutexAcquire(mutex_new_msg_pump_controller_handle, osWaitForever);
	number_pump = (uint8_t)atoi((char *)pump);
	strncpy((char *)liters_fuel, (char *)liters_dispache, strlen((char *)liters_dispache));

	if (!strcmp((char *)type, "Regular")) {
		type_fuel = REGULAR;
	}
	else if (!strcmp((char *)type, "Premium")) {
		type_fuel = PREMIUM;
	}
	else if (!strcmp((char *)type, "Regular diesel")) {
		type_fuel = REGULAR_DIESEL;
	}
	else {
		type_fuel = PREMIUM_DIESEL;
	}

	osMessageQueuePut(controller_pump_queueHandle, &msg, 0L, 0);
	osMutexRelease(mutex_new_msg_pump_controller_handle);
}

void Model::stop_dispatch_action(uint8_t *pump)
{
	uint8_t msg = STOP_PUMP;

	osMutexAcquire(mutex_new_msg_pump_controller_handle, osWaitForever);
	number_pump = (uint8_t)atoi((char *)pump);
	osMessageQueuePut(controller_pump_queueHandle, &msg, 0L, 0);
	osMutexRelease(mutex_new_msg_pump_controller_handle);
}
