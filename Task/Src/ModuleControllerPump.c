/*
 * ModuleControllerPump.c
 *
 *  Created on: Jan 10, 2021
 *      Author: Hanes
 */

#include "cmsis_os.h"

#include "main.h"
#include "ModuleControllerPump.h"
/* -------------------- Private global variable -------------------- */
/* Definitions for Controller_pump */
osThreadId_t Controller_pumpHandle;
const osThreadAttr_t Controller_pump_attributes = {
  .name = "Controller_pump",
  .priority = (osPriority_t) osPriorityNormal3,
  .stack_size = 1024 * 4
};
/* Definitions for controller_pump_queue */
osMessageQueueId_t controller_pump_queueHandle;
const osMessageQueueAttr_t controller_pump_queue_attributes = {
  .name = "controller_pump_queue"
};

static uint32_t fuel_filled = 0;

/* ------------------- Prototype private methods ------------------- */
/**
 * @brief Callback with functionality of pumps.
 *
 * @param[in]	argument	Argumment passed to task.
 */
void controller_pump_task(void *argument);


/* ----------------- Implementation private methods ---------------- */
void controller_pump_task(void *argument)
{

	for(;;) {
		osDelay(1);
	}
}

/* ----------------- Implementation public methods ----------------- */
bool_t module_controller_pump_started(void)
{
	  /* Create the queue(s) */
	  /* creation of controller_pump_queue */
	  controller_pump_queueHandle = osMessageQueueNew (2, sizeof(uint8_t), &controller_pump_queue_attributes);

	  if (controller_pump_queueHandle == NULL) {
		  return FALSE;
	  }

	  /* creation of Controller_pump */
	  Controller_pumpHandle = osThreadNew(controller_pump_task, NULL, &Controller_pump_attributes);

	  if (Controller_pumpHandle == NULL) {
		  return FALSE;
	  }

	  return TRUE;
}
