/*
 * ModuleNFC.h
 *
 *  Created on: 24 dic. 2020
 *      Author: Hanes
 */

#ifndef INC_MODULENFC_H_
#define INC_MODULENFC_H_

#include "main.h"

/**
 * @brief Function to initialize semaphore, queue and task
 * used for drive module NFC.
 *
 * @return Return 1 if was success or 0 in other case
 */
bool_t module_nfc_started(void);

/**
 * @brief Function to release semaphore to start initialize
 * module and task.
 */
void module_nfc_release_initialization(void);

#endif /* INC_MODULENFC_H_ */
