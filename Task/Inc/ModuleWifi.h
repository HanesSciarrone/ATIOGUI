/*
 * ModuleWifi.h
 *
 *  Created on: Aug 29, 2020
 *      Author: Hanes
 */

#ifndef INC_MODULEWIFI_H_
#define INC_MODULEWIFI_H_

#include "main.h"

/**
 * @defgroup DEFINE Define Wi-Fi
 *
 * @{
 */

/** Quantity maximum supported on list , must same element  quantity of list on GUI */
#define MAX_COUNT_LIST	30

/** Macro define maximum length of strings on ssid and password variable. This value depend of GUI */
#define MAX_LENGTH_CONNECTION_NETWORK	22

/** This value depend of GUI */
#define MAX_LENGTH_CREDENTIAL	20

/** Macro define maximum length of strings on credential variable */
#define MAX_LENGTH_MESSAGE_CREDENTIAL	MAX_LENGTH_CREDENTIAL + 4

/** @} */

enum command_type{
	EVENT_SEND_CREDENTIAL = 'A',
	EVENT_SEND_QUANTITY_SALE,
	EVENT_SEND_SALE_FINALIZATION,
	CMD_RESULT_CREDENTIAL,
	CMD_RESULT_SALE
};

enum result_validation_user{
	ERROR_DATA_RECEIVED,
	VALID_USER,
	INVALID_USER
};

/// Type of operation used on WifiModule
typedef enum
{
	SCAN_NETWORK,		/*!< Search for nearby networks */
	CONNECT_NETWORK,	/*!< Connect to specific network */
	SEND_CREDENTIAL,	/*!< Send credential to server */
	ANY_OPERATION		/*!< Don't any operation */
}WifiModule_Operation;

/// Type of variable that data's network
typedef struct
{
	uint8_t ssid[22];	/*!< Name of network */
	int8_t rssi;		/*!< Signal strength */
}ListNetwork_t;

/** Type of variable where result and message to Module
 * 	Wi-Fi will be save
 */
typedef struct
{
	ListNetwork_t listNetwork[MAX_COUNT_LIST];
	uint8_t ssid[MAX_LENGTH_CONNECTION_NETWORK];
	uint8_t password[MAX_LENGTH_CONNECTION_NETWORK];
	uint8_t data[MAX_LENGTH_MESSAGE_CREDENTIAL];
	uint8_t resultOperation;
}WifiMessage_t;



/**
 * @brief Function to initialize mutex, semaphore, queue and task
 * used for drive module Wi-Fi.
 *
 * @return Return 1 if was success or 0 in other case
 */
bool_t ModuleWifi_Started(void);

#endif /* INC_MODULEWIFI_H_ */
