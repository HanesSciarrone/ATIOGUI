/*
 * ESP8266.c
 *
 *  Created on: Apr 18, 2020
 *      Author: hanes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ESP8266.h"

#ifdef WIFI_RTOS
#include "cmsis_os.h"
#endif

#define AT_OK				"OK\r\n"
#define AT_ERROR			"ERROR\r\n"
#define AT_STATUS_OK		"STATUS:"
#define AT_FAIL_OPERATION	"FAIL\r\n"
#define AT_SEND_LENGTH_OK	"OK\r\n>"
#define AT_SEND_OK			"SEND OK\r\n"
#define AT_RECEIVE_STRING	"+IPD,"


#define AT_RES_BUSYP_STR	"busy p..."
#define AT_RES_BUSYS_STR	"busy s..."
#define AT_IPD_STRING_OK	"OK\r\n\r\n"


#define KEEPALIVE_CONNECTION	60UL

typedef enum
{
	FALSE,
	TRUE,
	false = 0,
	true
}ESP8266_bool_t;

/* Declaretion of global variable private */
uint8_t bufferRx[MAX_BUFFER_SIZE];
uint8_t bufferCmd[MAX_BUFFER_SIZE];
uint8_t tempBuffer[100];
uint32_t templength = 0;

ESP8266_CommInterface_s *commInterface;
/* -------------------------------------- */


static void ESP8266_Delay(uint32_t delay);
static uint16_t ESP8266_ParseLengthData(uint8_t *command, uint16_t size, const uint32_t lengthData);
static ESP8266_StatusTypeDef_t ESP8266_Sent(uint8_t *data, uint16_t length, const uint8_t *pattern, uint32_t timeout);
static ESP8266_StatusTypeDef_t ESP8266_SendCommand(uint16_t length, const uint8_t *pattern, uint32_t timeout);


static void ESP8266_Delay(uint32_t delay)
{
	#ifdef WIFI_RTOS
		osDelay(delay/portTICK_PERIOD_MS);
	#else
		uint32_t startTick;

		startTick = HAL_GetTick();
		while( HAL_GetTick() - startTick < delay );
	#endif
}

static uint16_t ESP8266_ParseLengthData(uint8_t *command, uint16_t size, const uint32_t lengthData)
{
	/*
	 * Warning: Is mandatory I say how many byte I will send and adding 2 because \r\n
	 * Possible response:
	 *
	 * Good response
	 * \r\nOK\r\n>
	 *
	 * Bad response
	 * link is not valid\r\n\r\nERROR\r\n
	 */
	int length = 0;
	if ( command == NULL ) {
		return 0;
	}

	strncpy((char *)command, "\0", size);
	strncpy((char *)command, "AT+CIPSEND=", 11);
	length = strlen((char *)command);
	itoa(lengthData, (char *)(command+length), 10);
	strncat((char *)command, "\r\n", 2);

	return strlen((char *)command);
}

static ESP8266_StatusTypeDef_t ESP8266_Sent(uint8_t *data, uint16_t length, const uint8_t *pattern, uint32_t timeout)
{
	uint32_t offset;
	uint8_t char_rx = 0;

	/*
	 * Possible response:
	 *
	 * Response fine
	 * busy s...\r\n\r\nRecv <length> Bytes\r\n\r\nSEND OK\r\n	(I sent more data that I said)
	 * Recv <length> Bytes\r\n\r\nSEND OK\r\n					(I sent same quantity of data that I said)
	 */

	if (commInterface->send(data, length) != 0) {
		return ESP8266_ERROR;
	}

	strncpy((char *)bufferRx, "\0", MAX_BUFFER_SIZE);
	offset = 0;

	while(commInterface->recv(&char_rx, 1, timeout) != 0 ) {
		bufferRx[offset++] = char_rx;

		if( offset == MAX_BUFFER_SIZE ) {
			offset = 0;
		}

		if ( strstr((char *)bufferRx, AT_ERROR) != NULL ) {
			return ESP8266_ERROR;
		}
		else if( strstr((char *)bufferRx, (const char *)pattern) != NULL ) {
			return ESP8266_OK;
		}
	}

	return ESP8266_ERROR;
}

static ESP8266_StatusTypeDef_t ESP8266_SendCommand(uint16_t length, const uint8_t *pattern, uint32_t timeout)
{
	uint32_t offset;
	uint8_t char_rx = 0;

	if (commInterface->send(bufferCmd, length) != 0) {
		return ESP8266_ERROR;
	}

	ESP8266_Delay(TIME_MS_MSG_RESPONSE);
	strncpy((char *)bufferRx, "\0", MAX_BUFFER_SIZE);
	offset = 0;

	while( commInterface->recv(&char_rx, 1, timeout) != 0 ) {
		bufferRx[offset++] = char_rx;

		if( offset == MAX_BUFFER_SIZE ) {
			offset = 0;
		}

		else if ( strstr((char *)bufferRx, AT_ERROR) != NULL ) {
			return ESP8266_ERROR;
		}
		else if( strstr((char *)bufferRx, (const char *)pattern) != NULL) {
			return ESP8266_OK;
		}
	}

	return ESP8266_ERROR;
}




ESP8266_StatusTypeDef_t ESP8266_CommInterface_Init(ESP8266_CommInterface_s *interface)
{
	if (interface == NULL)
	{
		return ESP8266_ERROR;
	}

	commInterface = interface;

	return ESP8266_OK;
}

ESP8266_StatusTypeDef_t ESP8266_restart(void)
{
	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+RST\r\n", 8);
	return ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 3000);
}

ESP8266_StatusTypeDef_t ESP8266_SetEcho(const uint8_t mode)
{
	/*
	 * Possible response of command:
	 *
	 * ATE<num>\r\r\n\r\nOK\r\n
	 * or
	 * ERROR\r\n
	 */

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	switch(mode) {
		case 1: {
			strncpy((char *)bufferCmd, "ATE1\r\n", 6);
		}
		break;

		default: {
			strncpy((char *)bufferCmd, "ATE0\r\n", 6);
		}
	}

	return ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 3000);
}

ESP8266_StatusTypeDef_t ESP8266_SetModeWIFI(uint8_t *mode)
{
	/*
	 * Possible response of command:
	 *
	 * \r\nOK\r\n
	 *
	 * or
	 *
	 * ERROR\r\n
	 *
	 * 1 : station mode
	 * 2 : softAP mode
	 * 3 : softAP + station mode
	 */

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CWMODE_CUR=", 14);
	strcat((char *)bufferCmd, (char *)mode);
	strcat((char *)bufferCmd, "\r\n");

	return ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 3000);
}

ESP8266_StatusTypeDef_t ESP8266_SetMultipleConnection(uint8_t *mode)
{
	/*
	 * Possible response of command:
	 *
	 * \r\nOK\r\n or ERROR\r\n.
	 * Also if you already connected, returns
	 *
	 * Link is builded
	 */

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CIPMUX=", 10);
	strcat((char *)bufferCmd , (char *)mode);
	strcat((char *)bufferCmd , "\r\n");

	return ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 3000);
}

ESP8266_StatusTypeDef_t ESP8266_SetListNeetwork(void)
{
	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CWLAPOPT=0,6\r\n", strlen("AT+CWLAPOPT=0,6\r\n"));

	return ESP8266_SendCommand(strlen("AT+CWLAPOPT=0,6\r\n"), (const uint8_t *)AT_OK, 3000);
}

ESP8266_StatusTypeDef_t ESP8266_ScanNetwork(void)
{
	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CWLAP\r\n", strlen("AT+CWLAP\r\n"));

	return ESP8266_SendCommand(strlen("AT+CWLAP\r\n"), (const uint8_t *)AT_OK, 5000);
}

ESP8266_StatusTypeDef_t ESP8266_StatusNetwork(void)
{
	/*
	 * Possible response of command:
	 *
	 * STATUS:<stat>\r\n\r\nOK\r\n or ERROR\r\n
	 *
	 * 2 : Got IP		(Conectod a la red)
	 * 3 : Connected 	(Conectado al servidor)
	 * 4 : Disconnected
	 * 5 : Wi-Fi connection fail
	 */

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CIPSTATUS\r\n", 14);


	if (ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 3000) == ESP8266_OK) {
		if (strstr((char *)bufferRx, "STATUS:2") != NULL || strstr((char *)bufferRx, "STATUS:3") != NULL) {
			return ESP8266_OK;
		}
		else {
			return ESP8266_ERROR;
		}
	}
	else {
		return ESP8266_ERROR;
	}
}

ESP8266_StatusTypeDef_t ESP8266_DisconnectAllNetwork(void)
{
	/*
	 * Possible response of command:
	 *
	 * \r\nOK\r\nWIFI DISCONNECT\r\n or ERROR\r\n
	 */
	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CWQAP\r\n", 10);

	return ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 3000);
}

ESP8266_StatusTypeDef_t ESP8266_ConnectionNetwork(const ESP8266_NetworkParameters_s *network)
{
	/*
	 * Possible response of command:
	 *
	 * WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n	(Operation success)
	 * WIFI DISCONNECT\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n	(Operation success)
	 * WIFI CONNECTED\r\n+CWJAP:1\r\n\r\nFAIL\r\nWIFI DISCONNECT\r\n	(Operation fail)
	 */

	if(network->ssid == NULL || network->password == NULL ) {
		return ESP8266_ERROR;
	}

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CWJAP_DEF=\"", 14);
	strncat((char *)bufferCmd, (char *)network->ssid, strlen((char *)network->ssid));
	strncat((char *)bufferCmd, "\",\"", 3);
	strncat((char *)bufferCmd, (char *)network->password, strlen((char *)network->password));
	strncat((char *)bufferCmd, "\"\r\n", 3);

	return ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 20000);
}

ESP8266_StatusTypeDef_t ESP8266_ConnectionServer(const ESP8266_ServerParameters_s *service)
{
	/*
	 * Possible response of command:
	 *
	 * CONNECT\r\n\r\nOK\r\n				(Se conecto bien al server)
	 * ALREADY CONNECTED\r\n\r\nERROR\r\n	(Ya esta conectado al servidor)
	 * no ip\r\n\r\nERROR\r\n				(No se conecto al server, no esta conectado a la red)
	 */

	uint16_t length;

	if (service->host == NULL || service->protocol == NULL || service->port <= 0 ) {
		return ESP8266_ERROR;
	}

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CIPSTART=\"", 13);
	strncat((char *)bufferCmd, (char *)service->protocol, strlen((char *)service->protocol));
	strncat((char *)bufferCmd, "\",\"", 3);
	strncat((char *)bufferCmd, (char *)service->host, strlen((char *)service->host));
	strncat((char *)bufferCmd, "\",", 2);
	length = strlen((char *)bufferCmd);
	itoa(service->port, (char *)(bufferCmd+length), 10);
	strncat((char *)bufferCmd, "\r\n", 2);

	if (ESP8266_SendCommand(strlen((char *)bufferCmd), (const uint8_t *)AT_OK, 10000) == ESP8266_ERROR) {
		if (strstr((char *)bufferRx, "ALREADY CONNECTED") != NULL) {
			return ESP8266_OK;
		}
		else {
			return ESP8266_ERROR;
		}
	}
	else {
		return ESP8266_OK;
	}
}

ESP8266_StatusTypeDef_t ESP8266_SentData(uint8_t *buffer, uint32_t length)
{
	uint32_t cmdLength = 0;
	ESP8266_StatusTypeDef_t status = ESP8266_OK;

	if (buffer == NULL || length == 0)
	{
		return ESP8266_ERROR;
	}

	cmdLength = ESP8266_ParseLengthData(bufferCmd, MAX_BUFFER_SIZE, length);
	status = ESP8266_SendCommand(cmdLength, (const uint8_t *)AT_SEND_LENGTH_OK, 2000);

	if( status != ESP8266_OK )
	{
		return ESP8266_ERROR;
	}

	ESP8266_Delay(500);
	return ESP8266_Sent(buffer, length, (const uint8_t *)AT_SEND_OK, 5000);
}

ESP8266_StatusTypeDef_t ESP8266_ReceiveData(uint8_t *buffer, uint32_t *length)
{
	uint32_t index, i = 0, length_data = 0;
	uint8_t char_rx = 0, length_frame[5];
	ESP8266_bool_t new_frame;

	/*
	 * When receive data to ESP8266 the frame will receive is:
	 *
	 * \r\n+IPD,<length>:<Data>
	 */

	/* Receive frame of data until we receive the last one.
	 *
	 * All frame start with string "+IPD,"
	 * Then receive length of frame, it's maximum value is 1460.
	 * Of the end receive data.
	 *
	 */
	*length = 0;
	strncpy((char *)bufferRx, "\0", MAX_BUFFER_SIZE);
	index = 0;
	new_frame = FALSE;

	while(1) {
		if( commInterface->recv(&char_rx, 1, 5000) != 0 ) {
			if( new_frame == TRUE ) {
				if( length_data-- ) {
					*buffer++ = char_rx;
					(*length)++;
				}
				else {
					strncpy((char *)bufferRx, "\0", MAX_BUFFER_SIZE);
					index = 0;
					new_frame = FALSE;
				}
			}
			bufferRx[index++] = char_rx;
		}
		else {
			/* There is error while I'm reading return error */
			if (new_frame == TRUE && length_data != 0) {
				return ESP8266_ERROR;
			}
			else
			{
				break;
			}
		}

		/* Check if there is overflow on buffer */
		if (index >= MAX_BUFFER_SIZE) {
			/* if there is a new frame and length of before frame isn't 0 return error */
			if (new_frame == TRUE && length_data != 0) {
				return ESP8266_ERROR;
			}
			else {
				break;
			}
		}

		/* Check if there is a new frame of data */
		if(strstr((char *)bufferRx, (const char *)AT_RECEIVE_STRING) != NULL && new_frame == FALSE) {
			// Extract the length of frame
			i = 0;
			strncpy((char *)length_frame, "\0", 5);
			do {
				commInterface->recv(&char_rx, 1, 5000);
				length_frame[i++] = char_rx;
			} while (char_rx != ':' && i < 4);

			length_data = atoi((char *)length_frame);
			new_frame = TRUE;
		}

		// Check if there is error
		if (strstr((char *)bufferRx, (const char *)AT_ERROR) != NULL) {
			return ESP8266_ERROR;
		}

		// Check is end of frame
		if (strstr((char *)bufferRx, (const char *)AT_IPD_STRING_OK) != NULL) {
			new_frame = FALSE;
		}
	}

	return ESP8266_OK;
}

ESP8266_StatusTypeDef_t ESP8266_ConnectionClose(void)
{
	/*
	 * Possible response of command:
	 *
	 * CLOSE\r\n\r\nOK\r\n
	 *\r\nERROR\r\n
	 */

	strncpy((char *)bufferCmd, "\0", MAX_BUFFER_SIZE);
	strncpy((char *)bufferCmd, "AT+CIPCLOSE\r\n", 13);

	return ESP8266_SendCommand(13, (const uint8_t *)AT_OK, 2000);
}

void ESP8266_GetModuleResponse(uint8_t *buffer, uint32_t length)
{
	strncpy((char *)buffer, "\0", length);
	strncpy((char *)buffer, (char *)bufferRx, strlen((char *)bufferRx));
}

//ESP8266_StatusTypeDef_t GenerateRequest(void *data)
//{
//	ESP8266_Datacompleted_s *information;
//	ESP8266_StatusTypeDef_t returnValue = ESP8266_OK;
//
//	MQTTPacket_connectData dataConnection = MQTTPacket_connectData_initializer;
//	MQTTString topicString = MQTTString_initializer;
//	uint8_t sessionPresent, connack_rc, buffer[200];
//	uint32_t length = 0, retry = 0;
//	static uint8_t message = 64;
//
//	information = (ESP8266_Datacompleted_s *)data;
//
//
//	switch (information->state)
//	{
//		case CONNECTION_NETWORK:
//		{
//			returnValue = ESP8266_ConnectionNetwork(information->dataNetwork);
//
//			if( returnValue == ESP8266_OK )
//			{
//				information->state = STATUS_NETWORK;
//			}
//		}
//		break;
//
//		case STATUS_NETWORK:
//		{
//
//			returnValue = ESP8266_StatusNetwork();
//
//			if( returnValue != ESP8266_OK )
//			{
//				information->state = CONNECTION_NETWORK;
//			}
//			else
//			{
//				information->state = ESTABLISH_CONNECTION;
//			}
//		}
//		break;
//
//		case ESTABLISH_CONNECTION:
//		{
//
//			returnValue = ESP8266_EstablichConnection(information->dataServer);
//
//			if (returnValue != ESP8266_OK)
//			{
//				information->state = STATUS_NETWORK;
//			}
//			else
//			{
//				information->state = SEND_CONNECT_MQTT;
//			}
//		}
//		break;
//
//		case SEND_CONNECT_MQTT:
//		{
//			retry = 0;
//			while( retry < 2 )
//			{
//				dataConnection.MQTTVersion = 3;
//				dataConnection.clientID.cstring = "hanes";
//				dataConnection.keepAliveInterval = KEEPALIVE_CONNECTION;
//				dataConnection.will.qos = 0;
//				memset(buffer, 0, sizeof(buffer));
//				length = MQTTSerialize_connect(buffer, sizeof(buffer), &dataConnection);
//				returnValue = ESP8266_MsgRequest(buffer, length);
//
//				if( returnValue != ESP8266_OK )
//				{
//					if( strstr((char *)bufferRx, "CLOSED\r\n") != NULL )
//					{
//						retry = 5;
//						information->state = ESTABLISH_CONNECTION;
//					}
//					else
//					{
//						information->state = CLOSE_CONNECTION;
//						retry++;
//						continue;
//					}
//
//				}
//
//				memset(buffer, 0, sizeof(buffer));
//				length = 0;
//				returnValue =  ESP8266_MsgReceiveData(buffer, &length);
//
//				if( returnValue != ESP8266_OK )
//				{
//					information->state = CLOSE_CONNECTION;
//				}
//				else
//				{
//					if ( MQTTDeserialize_connack(&sessionPresent, &connack_rc, buffer, strlen((char *)buffer)) != 1 )
//					{
//						information->state = CLOSE_CONNECTION;
//					}
//					else
//					{
//						information->state = SEND_SUBCRIBE_MQTT;
//						break;
//					}
//				}
//
//				retry++;
//			}
//		}
//		break;
//
//		case SEND_SUBCRIBE_MQTT:
//		{
//			retry = 0;
//			while (retry < 2)
//			{
//				MQTTString topicSubcribeString = MQTTString_initializer;
//				topicSubcribeString.cstring = "SUB_ESP8266";
//				int32_t requestQoS = 0;
//				length = MQTTSerialize_subscribe(buffer, sizeof(buffer), 0, 1, 1, &topicSubcribeString, (int *)&requestQoS);
//				returnValue = ESP8266_MsgRequest(buffer, length);
//
//				if( returnValue != ESP8266_OK )
//				{
//					if( strstr((char *)bufferRx, "CLOSED\r\n") != NULL )
//					{
//						retry = 5;
//						information->state = ESTABLISH_CONNECTION;
//					}
//					else
//					{
//						information->state = CLOSE_CONNECTION;
//						retry++;
//						continue;
//					}
//				}
//
//				memset(buffer, 0, sizeof(buffer));
//				length = 0;
//				returnValue =  ESP8266_MsgReceiveData(buffer, &length);
//				if( returnValue != ESP8266_OK )
//				{
//					information->state = CLOSE_CONNECTION;
//				}
//				else
//				{
//					uint16_t subcribe_MsgID;
//					int32_t subcribeCount, granted_QoS;
//					if ( MQTTDeserialize_suback(&subcribe_MsgID, 1, (int *)&subcribeCount, (int *)&granted_QoS, buffer, strlen((char *)buffer)) != 1 )
//					{
//						information->state = CLOSE_CONNECTION;
//					}
//					else
//					{
//						information->state = SEND_PUBLISH_MQTT;
//						break;
//					}
//				}
//
//				retry++;
//			}
//		}
//		break;
//
//
//		case SEND_PUBLISH_MQTT:
//		{
//			topicString.cstring = "ESP8266";
//
//			length = MQTTSerialize_publish(buffer, sizeof(buffer), 0, 0, 0, 0, topicString, (unsigned char*)&message, 1);
//			returnValue = ESP8266_MsgRequest(buffer, length);
//			if( message == 122 )
//			{
//				message = 64;
//			}
//			message++;
//			if( returnValue != ESP8266_OK )
//			{
//				information->state = CLOSE_CONNECTION;
//				break;
//			}
//			else
//			{
//				information->state = RECEIVE_MESSAGE;
//			}
//
//		}
//		break;
//
//		case RECEIVE_MESSAGE:
//		{
//			memset(buffer, 0, sizeof(buffer));
//			returnValue =  ESP8266_MsgReceiveData(buffer, &length);
//			if (returnValue != ESP8266_OK)
//			{
//				information->state = CLOSE_CONNECTION;
//			}
//			else
//			{
//				if( strlen((char *)buffer) != 0 )
//				{
//					uint8_t granted_Dup, granted_Retained, *granted_Payload;
//					int32_t granted_QoS, granted_PayloadLen;
//					uint16_t granted_PacketID;
//					MQTTString granted_TopicString;
//
//					if( MQTTDeserialize_publish(&granted_Dup, (int *)&granted_QoS, &granted_Retained,
//												&granted_PacketID, &granted_TopicString, (unsigned char **)&granted_Payload,
//												(int *)&granted_PayloadLen, (unsigned char *)buffer, (int)granted_PayloadLen) == 1)
//					{
//						information->state = SEND_UNSUBCRIBE_MQTT;
//					}
//					else
//					{
//						information->state = CLOSE_CONNECTION;
//					}
//				}
//			}
//		}
//		break;
//
//		case SEND_UNSUBCRIBE_MQTT:
//		{
//			memset(buffer, 0, sizeof(buffer));
//			MQTTSerialize_disconnect(buffer, sizeof(buffer));
//			returnValue = ESP8266_MsgRequest(buffer, length);
//
//			information->state = CLOSE_CONNECTION;
//		}
//		break;
//
//		case CLOSE_CONNECTION:
//		{
//			if( returnValue != ESP8266_OK )
//			{
//				ESP8266_Close();
//			}
//			else
//			{
//				returnValue = ESP8266_Close();
//			}
//
//			information->state = STATUS_NETWORK;
//			HAL_Delay(5000);
//		}
//		break;
//
//		default:
//		{
//			information->state = STATUS_NETWORK;
//		}
//	}
//
//
//	return returnValue;
//}

