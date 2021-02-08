	/*
 * Library.c
 *
 *  Created on: 16 ene. 2021
 *      Author: Hanes
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Library.h"

#define UPPERCASE_TO_LOWERCASE	0x20

/* Private  */

bool is_alphabetic(uint8_t character)
{
  if (character >= 'a' && character <= 'z') {
    return true;
  }

  if (character >= 'A' && character <= 'Z') {
    return true;
  }

  return false;
}

bool is_numeric(uint8_t character)
{
	if (character >= '0' && character <= '9') {
		return true;
	}

	return false;
}

void stolower(uint8_t *string)
{
	while (*string) {
		if (*string >= 'A' && *string <= 'Z') {
			*string += UPPERCASE_TO_LOWERCASE;
		}

		string++;
	}
}

void str_append(uint8_t *string, uint8_t character)
{
	uint8_t aux_str[2];

	aux_str[0] = character;
	aux_str[1] = 0;

	if (string == NULL) {
		return;
	}

	strcpy((char *)(string+strlen((char *)string)), (char *)aux_str);
}
