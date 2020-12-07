/*
 * ModuleNFC.c
 *
 *  Created on: Dec 6, 2020
 *      Author: Hanes
 */

#include "cmsis_os.h"
#include "PN532.h"
#include "ModuleWifi.h"
#include "ModuleNFC.h"


/* ------------------ Private global variables ------------------ */
static osThreadId_t nfc_serviceHandle;
const osThreadAttr_t nfc_service_attributes = {
  .name = "nfc_service",
  .priority = (osPriority_t) osPriorityNormal1,
  .stack_size = 1024 * 4
};
static pn532_interface nfc_interface;

/* ----------------- Prototype private functionz ---------------- */
static void nfc_task(void *argument);
static bool module_nfc_init(void);

/* USER CODE BEGIN Header_nfc_task */
/**
* @brief Function implementing the nfc_service thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_nfc_task */
void nfc_task(void *argument)
{
	uint8_t uid[8] = {0, 0, 0, 0, 0, 0, 0, 0}, length_uid;

	for(;;)
	{
		if (pn532_read_passive_target_id(&nfc_interface, PN532_MIFARE_ISO14443A, uid, &uid, 1000))
		{
			module_wifi_send_id(uid, length_uid);
			strncpy((char *)uid, "\0", 8);
			osDelay(1000/portTICK_PERIOD_MS);
		}

		osDelay(20/portTICK_PERIOD_MS);
	}
}

static bool module_nfc_init(void)
{
	uint32_t success;
	uint8_t retry = 2;

	nfc_interface.get_byte = &spi2_get_byte;
	nfc_interface.get_irq = &spi2_get_irq;
	nfc_interface.send_byte = &spi2_send_byte;
	nfc_interface.set_select = &spi2_set_select;

	do {
		success = pn532_get_firmware_version(&nfc_interface);
		retry--;
	} while(success == 0 && retry);

	if (success == 0)
	{
		return false;
	}

	// Set the max number of retry attempts to read from a card
	// This prevents us from waiting forever for a card, which is
	// the default behaviour of the PN532.
	success = pn532_set_passive_activation_retries(&nfc_interface, 0xFF);
	if( success == 0)
	{
		return false;
	}

	// configure board to read RFID tags
	success = pn532_sam_configuration(&nfc_interface);
	if( success == 0)
	{
		return false;
	}

	return true;
}

bool_t module_nfc_starter(void)
{
	if(!module_nfc_init()) {
		return FALSE;
	}
	/* creation of nfc_service */
	nfc_serviceHandle = osThreadNew(nfc_task, NULL, &nfc_service_attributes);

	if (nfc_serviceHandle == NULL) {
		return FALSE;
	}

	return TRUE;
}
