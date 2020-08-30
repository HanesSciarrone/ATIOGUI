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
 * @brief Function to initialize mutex, semaphore, queue and task
 * used for drive module Wi-Fi.
 *
 * @return Return 1 if was success or 0 in other case
 */
bool_t ModuleWifi_Started(void);

#endif /* INC_MODULEWIFI_H_ */
