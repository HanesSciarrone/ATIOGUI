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

/** @} */

/// Type of variable that data's network
typedef struct
{
	uint8_t ssid[22];	/*!< Name of network */
	int8_t rssi;	/*!< Signal strength */
}ListNetwork_t;

/// Type of operation used on WifiModule
typedef enum
{
	SCAN_NETWORK,		/*!< Search for nearby networks */
	CONNECT_NETWORK,	/*!< Connect to specific network */
	CONNECT_SERVER,		/*!< Connect to server */
	SEND_PACKET,		/*!< Send packet to server */
	CLOSE_SERVER		/*!< Close connection with server */
}WifiModule_Operation;

/**
 * @brief Function to initialize mutex, semaphore, queue and task
 * used for drive module Wi-Fi.
 *
 * @return Return 1 if was success or 0 in other case
 */
bool_t ModuleWifi_Started(void);

#endif /* INC_MODULEWIFI_H_ */
