/*
 * ModuleNFC.c
 *
 *  Created on: 24 dic. 2020
 *      Author: Hanes
 */

#include <stdbool.h>
#include <string.h>

#include "cmsis_os.h"

#include "main.h"
#include "ModuleNFC.h"
#include "SPI.h"
#include "PN532.h"

#define BUFFER_SIZE_UID	10

/* ----------------------- Global private variable ---------------------- */
/* Definitions for TaskNFC */
static osThreadId_t TaskNFCHandle;
static const osThreadAttr_t TaskNFC_attributes = {
  .name = "TaskNFC",
  .priority = (osPriority_t) osPriorityBelowNormal1,
  .stack_size = 256 * 4
};

/* Definitions for queue_nfc */
static osMessageQueueId_t queue_nfcHandle;
static const osMessageQueueAttr_t queue_nfc_attributes = {
  .name = "queue_nfc"
};
/* Definitions for nfc_init */
static osSemaphoreId_t nfc_initHandle;
static const osSemaphoreAttr_t nfc_init_attributes = {
  .name = "nfc_init"
};


/* ---------------------- Prototype private method ---------------------- */
/**
 * @brief Setup PN532 IC with parameters used to work it.
 *
 * @param[in]	driver	PN532 object with all functionality.
 *
 * @return Return true if was possible set up module or false in otherwise.
 */
static bool module_nfc_configure(pn532_drivers *driver);

/**
 * @brief Function to initialize object with pointer functions used to
 * communicate with PN532 (Module NFC).
 *
 * @param[in,out]	driver	Object with pointer function to initialize.
 */
static void module_nfc_comunication_driver_init(pn532_drivers *driver);

/**
 * @brief Function does all funtionality of NFC.
 */
static void service_nfc(void *argument);

/* -------------------- Implementation private method ------------------- */
static bool module_nfc_configure(pn532_drivers *driver)
{
	int retry = 0, version = 0;

	for (retry = 0; retry < 2; retry++) {
		version = pn532_get_firmware_version(driver);
		if (version != 0) {
			break;
		}
	}

	if (version == 0) {
		return false;
	}

	// Set the max number of retry attempts to read from a card
	// this prevents us from waiting forever for a card, which is
	// the default behaviour of the PN532.

	if(!pn532_set_passive_activation_retries(driver, 0xFF)) {
		return false;
	}

	// Configure board to read RFID tags
	if (!pn532_sam_config(driver)) {
		return false;
	}

	return true;
}

static void module_nfc_comunication_driver_init(pn532_drivers *driver)
{
	driver->get_byte = &spi_get_byte;
	driver->get_irq = &spi_get_irq;
	driver->send_byte = &spi_send_byte;
	driver->set_select = &spi_set_select;
}

void service_nfc(void *argument)
{
	uint8_t uid[BUFFER_SIZE_UID], length_uid = 0;
	pn532_drivers nfc_module;

	osSemaphoreAcquire(nfc_initHandle, osWaitForever);
	osSemaphoreDelete(nfc_initHandle);

	module_nfc_comunication_driver_init(&nfc_module);
	if (!module_nfc_configure(&nfc_module)) {
		Error_Handler();
	}

	for(;;){
		memset(uid, 0, BUFFER_SIZE_UID);
		length_uid = 0;
		if (pn532_read_passive_target_id(&nfc_module, PN532_MIFARE_ISO14443A, uid, &length_uid, 1000)) {
			// Cambiar la pantalla de carga.
			// LLamar a la function que envia al module Wifi.
			osDelay(1/portTICK_PERIOD_MS);
		}

		osDelay(50/portTICK_PERIOD_MS);
	}
}

/* -------------------- Implementation public method -------------------- */
bool_t ModuleNFC_Started(void)
{
	/* Creation of nfc_init */
	nfc_initHandle = osSemaphoreNew(1, 0, &nfc_init_attributes);
	if (nfc_initHandle == NULL) {
		return FALSE;
	}

	/* Creation of queue_nfc */
	queue_nfcHandle = osMessageQueueNew (2, sizeof(uint8_t), &queue_nfc_attributes);
	if (queue_nfcHandle == NULL) {
		return FALSE;
	}

	/* Creation of TaskNFC */
	TaskNFCHandle = osThreadNew(service_nfc, NULL, &TaskNFC_attributes);

	if (TaskNFCHandle == NULL) {
		return FALSE;
	}

	return TRUE;
}

void module_nfc_release_initialization(void)
{
	osSemaphoreRelease(nfc_initHandle);
}
