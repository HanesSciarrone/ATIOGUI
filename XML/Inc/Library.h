/*
 * Library.h
 *
 *  Created on: 16 ene. 2021
 *      Author: Hanes
 */

#ifndef INC_LIBRARY_H_
#define INC_LIBRARY_H_

#include <stdbool.h>
#include "stm32f7xx_hal.h"

bool is_alphabetic(uint8_t character);
bool is_numeric(uint8_t character);
void stolower(uint8_t *string);
void str_append(uint8_t *string, uint8_t character);

#endif /* INC_LIBRARY_H_ */
