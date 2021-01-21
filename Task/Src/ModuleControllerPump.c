/*
 * ModuleControllerPump.c
 *
 *  Created on: Jan 10, 2021
 *      Author: Hanes
 */
#include <stdlib.h>
#include <string.h>

#include "cmsis_os.h"

#include "main.h"
#include "Build_XML.h"
#include "XML_Parser.h"
#include "Library.h"
#include "ModuleControllerPump.h"

#define SIZE_MAX_BUFFER_COMMAND			1024

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

uint8_t  number_pump = 0;
uint8_t liters_fuel[20];
enum type_fuel_t type;


/* ------------------- Prototype private methods ------------------- */

/**
 * @brief Parser response of dispache command
 *
 * @param[in]	message		Buffer where message will be storage.
 * @param[in]	pump		Number pump used on command sent.
 * @param[in]	type		Type of fuel sent on command
 *
 * @return Return state of response.
 */
static state_pump_t receive_response_dispache_pump(uint8_t *message, uint8_t pump, enum type_fuel_t type);

/**
 * @brief Parser response of reset command
 *
 * @param[in]	message		Buffer where message will be storage.
 * @param[in]	pump		Number pump used on command sent.
 *
 * @return Return state of response.
 */
static state_pump_t receive_response_reset_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Send command to ask for state pump
 *
 * @param[in]	message		Buffer where message will be build.
 * @param[in]	pump		Number pump to restart.
 */
static void send_command_state_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Send command to stop pump
 *
 * @param[in]	message		Buffer where message will be build.
 * @param[in]	pump		Number pump to restart.
 */
static void send_command_stop_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Send command to dispache fuel with pump
 *
 * @param[in]	message		Buffer where message will be build.
 * @param[in]	pump		Number pump to restart.
 * @param[in]	liters		Liters to charge.
 * @param[in]	type_fuel	Type fuel to use.
 */
static void send_command_dispache_pump(uint8_t *message, uint8_t pump, uint8_t *liters, enum type_fuel_t type_fuel);

/**
 * @brief Send command to restart pump
 *
 * @param[in]	message	Buffer where message will be build.
 * @param[in]	pump	Number pump to restart.
 */
static void send_command_reset_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Callback with functionality of pumps.
 *
 * @param[in]	argument	Argumment passed to task.
 */
static void controller_pump_task(void *argument);


/* ----------------- Implementation private methods ---------------- */
static state_pump_t receive_response_stop_pump(uint8_t *message, uint8_t pump)
{

}

static state_pump_t receive_response_dispache_pump(uint8_t *message, uint8_t pump, enum type_fuel_t type)
{
	uint32_t index, index_attribute;
	uint16_t tag_size, content_size, size_attribute_name, size_attribute_value;
	uint8_t *xml, *content_element_root, *element, tag[50], content[50], attribute_name[50], attribute_value[50];

	// Function to receive data from UART

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>DISPACHE</COMMAN_ID>
	 *  	<NUMBER_PUMP>[Number pump]</NUMBER_PUMP>
	 *  	<RESPONSE type="type_fuel">[Response]</RESPONSE>
	 *  </CONTROLLER_CEM>
	 *
	 */

	// Skip declaration of XML
	xml = xml_skip_declaration(message);
	// Find all tags of root scheduler
	index = 0;
	content_element_root = xml_get_element_content_ptr(xml, content_size);
	while ((element = xml_get_element_by_index(content_element_root, index)) != NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		tag_size = xml_get_element_tag(element, tag);
		content_size = xml_get_element_content(element, content);

		if (!strcmp((char *)tag, "COMMAN_ID")) {
			// Ask if is response to reset command
			if (strcmp((char *)content, "DISPACHE")) {
				return PUMP_ERROR;
			}
		}
		else if (!strcmp((char *)tag, "NUMBER_PUMP")) {
			// Ask if response is for same number pump of command
			if (atoi((char *)content) != pump) {
				return PUMP_ERROR;
			}
		}
		else if (!strcmp((char *)tag, "RESPONSE")) {

			index_attribute = 0;
			while (xml_get_element_attribute_by_index(element, index_attribute, attribute_name, &size_attribute_name, attribute_value, &size_attribute_value)) {
				if (!strcmp((char *)attribute_name, "type")) {
					switch(type) {
					case REGULAR: {
						if (strcmp((char *)attribute_value, "REGULAR")) {
							return PUMP_ERROR;
						}
					}
					break;

					case PREMIUM: {
						if (strcmp((char *)attribute_value, "PREMIUM")) {
							return PUMP_ERROR;
						}
					}
					break;

					case REGULAR_DIESEL: {
						if (strcmp((char *)attribute_value, "REGULAR_DIESEL")) {
							return PUMP_ERROR;
						}
					}
					break;

					case PREMIUM_DIESEL: {
						if (strcmp((char *)attribute_value, "PREMIUM_DIESEL")) {
								return PUMP_ERROR;
						}
					}
					break;

					default: {
						return PUMP_ERROR;
					}
					break;

					}
				}

				index_attribute++;
			}

			// Ask if response is success
			if (!strcmp((char *)content, "OK")) {
				return PUMP_OK;
			}
			else if (!strcmp((char *)content, "EMPTHY")) {
				return PUMP_EMPTHY;
			}
			else if (!strcmp((char *)content, "INSUFFICIENT")) {
				return PUMP_INSUFFICIENT_FUEL;
			}
			else {
				return PUMP_ERROR;
			}
		}
		index++;
	}

	return PUMP_ERROR;
}

static state_pump_t receive_response_reset_pump(uint8_t *message, uint8_t pump)
{
	uint32_t index;
	uint16_t tag_size, content_size;
	uint8_t *xml, *content_element_root, *element, tag[50], content[50];

	// Function to receive data from UART

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>RESET</COMMAN_ID>
	 *  	<NUMBER_PUMP>[Number pump]</NUMBER_PUMP>
	 *  	<RESPONSE>[Response]</RESPONSE>
	 *  </CONTROLLER_CEM>
	 *
	 */

	// Skip declaration of XML
	xml = xml_skip_declaration(message);

	// Find all tags of root scheduler
	index = 0;
	content_element_root = xml_get_element_content_ptr(xml, content_size);
	while ((element = xml_get_element_by_index(content_element_root, index)) != NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		tag_size = xml_get_element_tag(element, tag);
		content_size = xml_get_element_content(element, content);

		if (!strcmp((char *)tag, "COMMAN_ID")) {
			// Ask if is response to reset command
			if (strcmp((char *)content, "RESET")) {
				return PUMP_ERROR;
			}
		}
		else if (!strcmp((char *)tag, "NUMBER_PUMP")) {
			// Ask if response is for same number pump of command
			if (atoi((char *)content) != pump) {
				return PUMP_ERROR;
			}
		}
		else if (!strcmp((char *)tag, "RESPONSE")) {
			// Ask if response is success
			if (!strcmp((char *)content, "OK")) {
				return PUMP_OK;
			}
			else {
				return PUMP_ERROR;
			}
		}
		index++;
	}

	return PUMP_ERROR;
}

static void send_command_state_pump(uint8_t *message, uint8_t pump)
{
	uint8_t number[4];
	xml_header_t *header;
	xml_element_t *root, *children_1, *children_2;

	// Initialize variable
	header = NULL;
	root = children_1 = children_2 = NULL;
	memset(number, 0, sizeof(number));

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>STATE</COMMAN_ID>
	 *  	<NUMBER_PUMP>number_pump</NUMBER_PUMP>
	 *  </CONTROLLER_CEM>
	 *
	 */

	header = BuildXML_NewXmlHeader();
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"version", (const uint8_t *)"1.0");
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"encoding", (const uint8_t *)"UTF-8");

	root = BuildXML_Newxml((const uint8_t *)"CONTROLLER_CEM");
	children_1 = BuildXML_Newxml((const uint8_t *)"COMMAN_ID");
	children_2 = BuildXML_Newxml((const uint8_t *)"NUMBER_PUMP");

	// Set bodies
	BuildXML_AddBody(children_1, (const uint8_t *)"STATE");
	BuildXML_AddBody(children_2, (const uint8_t *)itoa(pump, (char *)number, 10));

	// Set up children into root XML
	BuildXML_AddChildren(root, children_1);
	BuildXML_AddChildren(root, children_2);

	// Build XML inside string
	BuildXML_FormatHeader(header, message);
	BuildXML_Format(root, message);

	// Se envia los datos por UART

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);
}

static void send_command_stop_pump(uint8_t *message, uint8_t pump)
{
	uint8_t number[4];
	xml_header_t *header;
	xml_element_t *root, *children_1, *children_2;

	// Initialize variable
	header = NULL;
	root = children_1 = children_2 = NULL;
	memset(number, 0, sizeof(number));

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>STOP</COMMAN_ID>
	 *  	<NUMBER_PUMP>number_pump</NUMBER_PUMP>
	 *  </CONTROLLER_CEM>
	 *
	 */

	header = BuildXML_NewXmlHeader();
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"version", (const uint8_t *)"1.0");
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"encoding", (const uint8_t *)"UTF-8");

	root = BuildXML_Newxml((const uint8_t *)"CONTROLLER_CEM");
	children_1 = BuildXML_Newxml((const uint8_t *)"COMMAN_ID");
	children_2 = BuildXML_Newxml((const uint8_t *)"NUMBER_PUMP");

	// Set bodies
	BuildXML_AddBody(children_1, (const uint8_t *)"STOP");
	BuildXML_AddBody(children_2, (const uint8_t *)itoa(pump, (char *)number, 10));

	// Set up children into root XML
	BuildXML_AddChildren(root, children_1);
	BuildXML_AddChildren(root, children_2);

	// Build XML inside string
	BuildXML_FormatHeader(header, message);
	BuildXML_Format(root, message);

	// Se envia los datos por UART

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);
}

static void send_command_dispache_pump(uint8_t *message, uint8_t pump, uint8_t *liters, enum type_fuel_t type_fuel)
{
	uint8_t number[4];
	xml_header_t *header;
	xml_element_t *root, *children_1, *children_2, *children_3;

	// Initialize variable
	header = NULL;
	root = children_1 = children_2 = NULL;
	memset(number, 0, sizeof(number));

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>DISPACHE</COMMAN_ID>
	 *  	<NUMBER_PUMP>number_pump</NUMBER_PUMP>
	 *  	<FUEL type="type_fuel">liters_fuel</FUEL>
	 *  </CONTROLLER_CEM>
	 *
	 */

	header = BuildXML_NewXmlHeader();
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"version", (const uint8_t *)"1.0");
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"encoding", (const uint8_t *)"UTF-8");

	root = BuildXML_Newxml((const uint8_t *)"CONTROLLER_CEM");
	children_1 = BuildXML_Newxml((const uint8_t *)"COMMAN_ID");
	children_2 = BuildXML_Newxml((const uint8_t *)"NUMBER_PUMP");
	children_3 = BuildXML_Newxml((const uint8_t *)"FUEL");

	// Set attribute
	switch (type_fuel) {
	case REGULAR: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Regutal");
	}
	break;

	case PREMIUM: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Premium");
	}
	break;

	case REGULAR_DIESEL: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Regutal_diesel");
	}
	break;

	case PREMIUM_DIESEL: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Premium_diesel");
	}
	break;
	}

	// Set bodies
	BuildXML_AddBody(children_1, (const uint8_t *)"DISPACHE");
	BuildXML_AddBody(children_2, (const uint8_t *)itoa(pump, (char *)number, 10));
	BuildXML_AddBody(children_3, liters);

	// Set up children into root XML
	BuildXML_AddChildren(root, children_1);
	BuildXML_AddChildren(root, children_2);
	BuildXML_AddChildren(root, children_3);

	// Build XML inside string
	BuildXML_FormatHeader(header, message);
	BuildXML_Format(root, message);

	// Se envia los datos por UART

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);
}

static void send_command_reset_pump(uint8_t *message, uint8_t pump)
{
	uint8_t number[4];
	xml_header_t *header;
	xml_element_t *root, *children_1, *children_2;

	// Initialize variable
	header = NULL;
	root = children_1 = children_2 = NULL;
	memset(number, 0, sizeof(number));

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>RESET</COMMAN_ID>
	 *  	<NUMBER_PUMP>number_pump</NUMBER_PUMP>
	 *  </CONTROLLER_CEM>
	 *
	 */

	header = BuildXML_NewXmlHeader();
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"version", (const uint8_t *)"1.0");
	BuildXML_AddAttributeHeader(header, (const uint8_t *)"encoding", (const uint8_t *)"UTF-8");

	root = BuildXML_Newxml((const uint8_t *)"CONTROLLER_CEM");
	children_1 = BuildXML_Newxml((const uint8_t *)"COMMAN_ID");
	children_2 = BuildXML_Newxml((const uint8_t *)"NUMBER_PUMP");

	// Set bodies
	BuildXML_AddBody(children_1, (const uint8_t *)"RESET");
	BuildXML_AddBody(children_2, (const uint8_t *)itoa(pump, (char *)number, 10));

	// Set up children into root XML
	BuildXML_AddChildren(root, children_1);
	BuildXML_AddChildren(root, children_2);

	// Build XML inside string
	BuildXML_FormatHeader(header, message);
	BuildXML_Format(root, message);

	// Se envia los datos por UART

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);
}

static void controller_pump_task(void *argument)
{
	uint8_t msg;
	uint8_t message[SIZE_MAX_BUFFER_COMMAND];

	for(;;) {

		osMessageQueueGet(controller_pump_queueHandle, &msg, 0L, osWaitForever);

		memset(message, 0, sizeof(message));

		switch(msg) {

		case RESTART_PUMP: {
			send_command_reset_pump(message, number_pump);
			memset(message, 0, sizeof(message));
			receive_response_reset_pump(message, number_pump);
		}
		break;

		case DISPACHE_PUMP: {
			memset(liters_fuel, 0, sizeof(liters_fuel));
			send_command_dispache_pump(message, number_pump, liters_fuel, type);
			memset(message, 0, sizeof(message));
			receive_response_dispache_pump(message, number_pump, type);
		}
		break;

		case STOP_PUMP: {
			send_command_stop_pump(message, number_pump);
			receive_response_stop_pump(message, number_pump);
		}
		break;

		case STATE_PUMP: {
			send_command_state_pump(message, number_pump);
			//receive_response_state_pump();
		}
		break;

		}

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
