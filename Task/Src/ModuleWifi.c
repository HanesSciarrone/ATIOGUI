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
#include "MQTTPacket.h"

/* Include of module driver */
#include "ESP8266.h"

enum mqtt_state {
	STATUS_CONNECTION,
	CONNECT_WIFI,
	CONNECT_SERVER,
	SEND_CONNECT_BROKER,
	SUBSCRIBE_MQTT,
	SEND_DATA_MQTT,
	RECEIVE_DATA_MQTT,
	UNSUBSCRIBE_MQTT,
	SEND_DISCONNECT_BROKER,
	CLOSE_CONNECTION,
	END_OPERATION
};

/* Private macros ----------------------------------------------------------- */
// URL where is the broker MQTT
#define HOST						"broker.hivemq.com"
// Port where broker is listening to
#define PORT						1883
// Parameter connection of broker for keep alive
#define KEEPALIVE_CONNECTION		60UL
// Subscribe topic
#define SUBSCRIBE_TOPIC				"SUB_CESE"
// Publish topic
#define PUBLISH_TOPIC				"PUB_CESE"

#define TIME_MS_CONNECTION			2000
/* Timeout before try to connect with server or reconnect with network wi-fi */
#define TIME_MS_ESTABLISH_SERVER	3000

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
 * @brief Sent message to unsubscribe of topic. Retry until 3 time
 *
 * @return Return the follow state on finite state machine depending
 * of result of operation.
 */
static enum mqtt_state sent_unsubscribe_mqtt(void);

/**
 * @brief Receive message from broker MQTT.
 *
 * @param[in]	data	Buffer where data received will be storage
 *
 * @return Return the follow state on finite state machine depending
 * of result of operation.
 */
static enum mqtt_state receive_data_mqtt(uint8_t *data);

/**
 * @brief Sent message with data  to broker MQTT.
 *
 * @param[in]	data	Information that you would like to sent.
 *
 * @return Return the follow state on finite state machine depending
 * of result of operation.
 */
static enum mqtt_state sent_data_mqtt(uint8_t *data);

/**
 * @brief Sent message of subscribe to broker MQTT.
 *
 * @return Return the follow state on finite state machine depending
 * of result of operation.
 */
static enum mqtt_state sent_subcribe_mqtt(void);

/**
 * @brief Sent message of connect to broker MQTT.
 *
 * @return Return the follow state on finite state machine depending
 * of result of operation.
 */
static enum mqtt_state sent_connect_mqtt(void);

/**
 * @brief Parse message received of response to credential from broker MQTT.
 *
 * @return Return type of user
 */
static enum result_validation_user parse_result_credential(void);

/**
 * @brief Send credential to broker MQTT
 *
 * @param[in]	data	Data that will be send to broker
 */
static void send_credential(uint8_t *data);

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
static enum mqtt_state sent_disconnect_mqtt(void)
{
	uint32_t length;
	uint8_t buffer[200];
	static uint8_t retry = 0;
	ESP8266_StatusTypeDef_t status;

	strncpy((char *)buffer, "\0", sizeof(buffer));
	length = MQTTSerialize_disconnect(buffer, sizeof(buffer));
	status = ESP8266_SentData(buffer, length);

	if (status == ESP8266_OK) {
		retry = 0;
		return CLOSE_CONNECTION;
	}

	retry++;
	osDelay(200/portTICK_PERIOD_MS);
	if (retry == 3) {
		retry = 0;
		return CLOSE_CONNECTION;
	}

	return SEND_DISCONNECT_BROKER;
}

static enum mqtt_state sent_unsubscribe_mqtt(void)
{
	uint32_t length;
	uint16_t packet_id;
	uint8_t buffer[200];
	static uint8_t retry = 0;
	ESP8266_StatusTypeDef_t status;
	MQTTString topicSubcribeString = MQTTString_initializer;

	topicSubcribeString.cstring = SUBSCRIBE_TOPIC;
	strncpy((char *)buffer, "\0", sizeof(buffer));
	length = MQTTSerialize_unsubscribe(buffer, sizeof(buffer), 0, 2, 1, &topicSubcribeString);
	status = ESP8266_SentData(buffer, length);

	if (status == ESP8266_OK) {
		// Receive response of subscribe
		strncpy((char *)buffer, "\0", sizeof(buffer));
		length = 0;
		status =  ESP8266_ReceiveData(buffer, &length);

		if (status == ESP8266_OK) {
			packet_id = 0;// should be the same as the packetid above, = 2
	        if (MQTTDeserialize_unsuback(&packet_id, buffer, strlen((char *)buffer)) == 1)
	        {
	        	retry = 0;
	        	return SEND_DISCONNECT_BROKER;
	        }
		}
	}

	retry++;
	osDelay(200/portTICK_PERIOD_MS);

	if (retry == 3) {
		retry = 0;
		return SEND_DISCONNECT_BROKER;
	}

	return UNSUBSCRIBE_MQTT;
}

static enum mqtt_state receive_data_mqtt(uint8_t *data)
{
	uint32_t length;
	int32_t granted_QoS, granted_PayloadLen;
	uint16_t granted_PacketID;
	uint8_t buffer[200], granted_Dup, granted_Retained, *granted_Payload;
	ESP8266_StatusTypeDef_t status;
	MQTTString granted_TopicString;

	strncpy((char *)buffer, "\0", sizeof(buffer));
	length = 0;
	status =  ESP8266_ReceiveData(buffer, &length);

	if (status == ESP8266_OK) {
		MQTTDeserialize_publish(&granted_Dup, (int *)&granted_QoS, &granted_Retained, &granted_PacketID, &granted_TopicString, (unsigned char **)&granted_Payload, (int *)&granted_PayloadLen, (unsigned char *)buffer, (int)granted_PayloadLen);

		if (granted_PayloadLen <= (MAX_LENGTH_MESSAGE_CREDENTIAL-1)) {
			strncpy((char*)data, (char *)granted_Payload, granted_PayloadLen);
		}
	}

	osDelay(200/portTICK_PERIOD_MS);
	return UNSUBSCRIBE_MQTT;
}

static enum mqtt_state sent_data_mqtt(uint8_t *data)
{
	uint32_t length;
	uint8_t buffer[200];
	enum mqtt_state state = RECEIVE_DATA_MQTT;
	ESP8266_StatusTypeDef_t status;
	MQTTString topicString = MQTTString_initializer;

	topicString.cstring = PUBLISH_TOPIC;
	length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, 0, 0, 0, topicString, (unsigned char*)data, strlen((char *)data));
	status = ESP8266_SentData(buffer, length);

	if (status != ESP8266_OK) {
		state = UNSUBSCRIBE_MQTT;
	}

	osDelay(200/portTICK_PERIOD_MS);
	return state;
}

static enum mqtt_state sent_subcribe_mqtt(void)
{
	uint32_t length;
	int32_t request_qos, subcribeCount, granted_QoS;;
	uint16_t subcribe_MsgID;
	uint8_t retry, buffer[200], moduleResponse[MAX_BUFFER_SIZE];
	enum mqtt_state state = SEND_DATA_MQTT;
	ESP8266_StatusTypeDef_t status;

	retry = 0;
	while (retry < 3) {
		MQTTString topicSubcribeString = MQTTString_initializer;
		topicSubcribeString.cstring = SUBSCRIBE_TOPIC;
		request_qos = 0;

		// Build and sent message of subcribe
		length = MQTTSerialize_subscribe(buffer, sizeof(buffer), 0, 1, 1, &topicSubcribeString, (int *)&request_qos);
		status = ESP8266_SentData(buffer, length);

		// Ask if operation failed
		if (status != ESP8266_OK) {
			ESP8266_GetModuleResponse(moduleResponse, MAX_BUFFER_SIZE);
			if( strstr((char *)moduleResponse, "CLOSED\r\n") != NULL )
			{
				retry = 3;
				state = CONNECT_WIFI;
			}
			else
			{
				state = SEND_DISCONNECT_BROKER;
			}
		}
		else {
			// Receive response of subscribe
			strncpy((char *)buffer, "\0", sizeof(buffer));
			length = 0;
			status =  ESP8266_ReceiveData(buffer, &length);

			if (status != ESP8266_OK) {
				state = SEND_DISCONNECT_BROKER;
			}
			else {
				if (MQTTDeserialize_suback(&subcribe_MsgID, 1, (int *)&subcribeCount, (int *)&granted_QoS, buffer, strlen((char *)buffer)) != 1 ) {
					state = SEND_DISCONNECT_BROKER;
				}
				else {
					state = SEND_DATA_MQTT;
					retry = 3;
				}
			}
		}

		retry++;
		osDelay(200/portTICK_PERIOD_MS);
	}

	return state;
}

static enum mqtt_state sent_connect_mqtt(void)
{
	uint32_t length;
	uint8_t retry = 0, moduleResponse[MAX_BUFFER_SIZE];
	uint8_t sessionPresent, connack_rc, buffer[200];
	enum mqtt_state state = SUBSCRIBE_MQTT;
	ESP8266_StatusTypeDef_t status;
	MQTTPacket_connectData dataConnection = MQTTPacket_connectData_initializer;

	retry = 0;
	while(retry < 3) {
		// Initialize data
		dataConnection.MQTTVersion = 3;
		dataConnection.clientID.cstring = "Hanes";
		dataConnection.keepAliveInterval = KEEPALIVE_CONNECTION;
		dataConnection.will.qos = 0;
		strncpy((char *)buffer, "0", sizeof(buffer));

		// Build message
		length = MQTTSerialize_connect(buffer, sizeof(buffer), &dataConnection);
		status = ESP8266_SentData(buffer, length);

		if (status != ESP8266_OK) {
			ESP8266_GetModuleResponse(moduleResponse, MAX_BUFFER_SIZE);

			// Ask if module is disconnected
			if (strstr((char *)moduleResponse, "CLOSED\r\n") != NULL) {
				retry = 3;
				state = CONNECT_WIFI;
			}
			else
			{
				retry++;
				state = SEND_DISCONNECT_BROKER;
			}
		}
		else {
			strncpy((char *)buffer, "\0", sizeof(buffer));
			length = 0;
			status = ESP8266_ReceiveData(buffer, &length);

			if (status != ESP8266_OK) {
				state = SEND_DISCONNECT_BROKER;
			}
			else {
				// Ask if I receive ACK of broker
				if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buffer, strlen((char *)buffer)) != 1 ) {
					state = SEND_DISCONNECT_BROKER;
				}
				else {
					state = SUBSCRIBE_MQTT;
					retry = 3;
				}
			}

			retry++;
		}

		osDelay(200/portTICK_PERIOD_MS);
	}

	return state;
}

static enum result_validation_user parse_result_credential(void)
{
	uint8_t index, offset, liters[21];
	/*
	 * Format of mesage is:
	 *
	 * [CommandID]|[Valid user]|[Quantity liters available]
	 *
	 * Where each data is separate for |
	 */

	// Ask if command received is correct
	if (wifiParameters.data[0] != CMD_RESULT_CREDENTIAL) {
		return ERROR_DATA_RECEIVED;
	}

	//Ask if user if invalid
	if (wifiParameters.data[2] == '0') {
		return INVALID_USER;
	}

	index = 4;
	offset = 0;
	strncpy((char *)liters, "\0", 21);
	while(wifiParameters.data[index] >= '0' && wifiParameters.data[index] <= '9' && index < 23) {
		liters[offset] = wifiParameters.data[index];
		index++;
		offset++;
	}

	memset(wifiParameters.data, 0, MAX_LENGTH_MESSAGE_CREDENTIAL);
	strncpy((char *)wifiParameters.data, (char *)liters, strlen((char *)liters));

	return VALID_USER;
}

static void send_credential(uint8_t *data)
{
	ESP8266_StatusTypeDef_t status;
	enum mqtt_state state = STATUS_CONNECTION;

	while(state != END_OPERATION) {
		switch(state) {
		// Check status connection
		case STATUS_CONNECTION: {
			status = ESP8266_StatusNetwork();
			if (status != ESP8266_OK) {
				state = CONNECT_WIFI;
				osDelay(TIME_MS_ESTABLISH_SERVER/portTICK_PERIOD_MS);
			}
			else {
				state = CONNECT_SERVER;
				osDelay(200/portTICK_PERIOD_MS);
			}
		}
		break;

		// Connect to Wifi
		case CONNECT_WIFI: {
			ESP8266_NetworkParameters_s network;

			network.ssid = wifiParameters.ssid;
			network.password = wifiParameters.password;
			status = ESP8266_ConnectionNetwork(&network);

			if (status == ESP8266_OK) {
				state = STATUS_CONNECTION;
				osDelay(TIME_MS_ESTABLISH_SERVER/portTICK_PERIOD_MS);
			}
			else {
				osDelay(TIME_MS_CONNECTION/portTICK_PERIOD_MS);
			}
		}
		break;

		// Connect to server
		case CONNECT_SERVER: {
			ESP8266_ServerParameters_s service;

			service.host = (uint8_t *)HOST;
			service.port = PORT;
			service.protocol = (uint8_t *)"TCP";
			status = ESP8266_ConnectionServer(&service);

			if (status != ESP8266_OK) {
				state = STATUS_CONNECTION;
				osDelay(TIME_MS_ESTABLISH_SERVER/portTICK_PERIOD_MS);
			}
			else {
				state = SEND_CONNECT_BROKER;
				osDelay(200/portTICK_PERIOD_MS);
			}
		}
		break;

		// Sent connect to MQTT
		case SEND_CONNECT_BROKER: {
			state = sent_connect_mqtt();
		}
		break;

		// Sent Subscribe MQTT
		case SUBSCRIBE_MQTT: {
			state = sent_subcribe_mqtt();
		}
		break;

		// Sent message
		case SEND_DATA_MQTT: {
			state = sent_data_mqtt(data);
		}
		break;

		// Receive data from broker MQTT
		case RECEIVE_DATA_MQTT: {
			memset(data, 0, MAX_LENGTH_MESSAGE_CREDENTIAL*sizeof(uint8_t));
			state = receive_data_mqtt(data);
		}
		break;

		// Sent unsubscribe MQTT
		case UNSUBSCRIBE_MQTT: {
			state = sent_unsubscribe_mqtt();
		}
		break;

		case SEND_DISCONNECT_BROKER: {
			state = sent_disconnect_mqtt();
		}
		break;

		case CLOSE_CONNECTION: {
			status = ESP8266_ConnectionClose();
			state = END_OPERATION;
		}
		break;

		default:
			state = END_OPERATION;

		}
	}
}

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

	osDelay(5000/portTICK_PERIOD_MS);

	/* Initialization of module ESP8266 */
	if (!WifiModule_Init())
	{
		osMutexDelete(mutex_NewMsg_WifiHandle);
		osThreadTerminate(TaskWifiHandle);
	}

	operation = ANY_OPERATION;

	while(1) {
		osMessageQueueGet(queue_Wifi_operationHandle,&operation, 0L, osWaitForever);

		switch(operation) {
			case SCAN_NETWORK: {
				ModuleWifi_ScanNetwork();
				msgGUI = 0;
				osMessageQueuePut(queue_NewMsg_GUI, &msgGUI, 0L, osWaitForever);
			}
			break;

			case CONNECT_NETWORK: {
				ESP8266_NetworkParameters_s network;
				ESP8266_StatusTypeDef_t status;

				status = ESP8266_DisconnectAllNetwork();
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);

				network.ssid = wifiParameters.ssid;
				network.password = wifiParameters.password;
				status = ESP8266_ConnectionNetwork(&network);

				if (status == ESP8266_OK) {
					wifiParameters.resultOperation = 1;
				}
				else {
					wifiParameters.resultOperation = 0;
				}

				osMutexRelease(mutex_NewMsg_WifiHandle);

				msgGUI = 1;
				osMessageQueuePut(queue_NewMsg_GUI, &msgGUI, 0L, osWaitForever);
			}
			break;

			case SEND_CREDENTIAL: {
				osMutexAcquire(mutex_NewMsg_WifiHandle, osWaitForever);
				send_credential(wifiParameters.data);
				msgGUI = 2;
				wifiParameters.resultOperation = parse_result_credential();
				osMessageQueuePut(queue_NewMsg_GUI, &msgGUI, 0L, osWaitForever);
				osMutexRelease(mutex_NewMsg_WifiHandle);
			}
			break;

			default:
				operation = ANY_OPERATION;
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
