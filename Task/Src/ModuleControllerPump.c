/*
 * ModuleControllerPump.c
 *
 *  Created on: Jan 10, 2021
 *      Author: Hanes
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cmsis_os.h"

#include "main.h"
#include "UART.h"
#include "Build_XML.h"
#include "XML_Parser.h"
#include "Library.h"
#include "ModuleControllerPump.h"

#define SIZE_MAX_BUFFER_COMMAND			1024
#define TIMEOUT_RESPONSE				1000

struct status_pump_s {
	uint8_t fuel_tank[30];
	uint8_t fuel_filled[30];
	enum type_fuel_t type_fuel;
	bool filling;
}status_pump;

typedef StaticSemaphore_t osStaticMutexDef_t;

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

/* Definitions for mutex_new_msg_pump_controller */
osMutexId_t mutex_new_msg_pump_controller_handle;
osStaticMutexDef_t mutex_new_msg_pump_controller_control_block;
const osMutexAttr_t mutex_new_msg_pump_controller_attributes = {
  .name = "mutex_new_msg_pump_controller",
  .cb_mem = &mutex_new_msg_pump_controller_control_block,
  .cb_size = sizeof(mutex_new_msg_pump_controller_control_block),
};

uint8_t number_pump = 0;
uint8_t liters_fuel[20];
enum type_fuel_t type_fuel;


/* ------------------- Prototype private methods ------------------- */

/**
 * @brief Compare fuel type of message with tipe of fuel used.
 *
 * @param[in]	string	Fuel type received on message.
 * @param[in]	type	Fuel type send on command.
 *
 * @return Return true if fuels are same or false in otherwise.
 */
static bool validate_type_fuel(uint8_t *string, enum type_fuel_t type);

/**
 * @brief Navigate data structure XML received on status pump response.
 *
 * @param[in]	root_content	String with data structure received.
 * @param[in]	type			Type of fuel used on command send.
 *
 * @return Return true if was success or false in otherwise.
 */
static bool get_status_parameters_pump(uint8_t *root_content, enum type_fuel_t type);

/**
 * @brief Parser status response atribute of STATUS tag on response
 * of status pump command.
 *
 * @param[in]	element	Element of XML with attribute
 *
 * @return Retunr true if response was success or false in otherwise.
 */
static bool get_attribute_status_tag(uint8_t *element);

/**
 * @brief Parser response of query state pump command
 *
 * @param[in]	message		Buffer where message will be storage.
 * @param[in]	pump		Number pump used on command sent.
 * @param[in]	type		Type of fuel sent on command
 *
 * @return Return state of response.
 */
static state_pump_t receive_response_state_pump(uint8_t *message, uint8_t pump, enum type_fuel_t type);

/**
 * @brief Parser response of stop command
 *
 * @param[in]	message		Buffer where message will be storage.
 * @param[in]	pump		Number pump used on command sent.
 *
 * @return Return state of response.
 */
static state_pump_t receive_response_stop_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Parser response of dispache command
 *
 * @param[in]	message		Buffer where message will be storage.
 * @param[in]	pump		Number pump used on command sent.
 * @param[in]	type		Type of fuel sent on command
 *
 * @return Return state of response.
 */
static state_pump_t receive_response_dispatch_pump(uint8_t *message, uint8_t pump, enum type_fuel_t type);

/**
 * @brief Parser response of reset command
 *
 * @param[in]	message		Buffer where message will be storage.
 * @param[in]	pump		Number pump used on command sent.
 *
 * @return Return state of response.
 */
static state_pump_t receive_response_start_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Send command to ask for state pump
 *
 * @param[in]	message		Buffer where message will be build.
 * @param[in]	pump		Number pump to restart.
 *
 * @return Return true if was success or false in otherwise.
 */
static bool send_command_state_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Send command to stop pump
 *
 * @param[in]	message		Buffer where message will be build.
 * @param[in]	pump		Number pump to restart.
 *
 * @return Return true if was success or false in otherwise.
 */
static bool send_command_stop_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Send command to dispache fuel with pump
 *
 * @param[in]	message		Buffer where message will be build.
 * @param[in]	pump		Number pump to restart.
 * @param[in]	liters		Liters to charge.
 * @param[in]	type_fuel	Type fuel to use.
 *
 * @return Return true if was success or false in otherwise.
 */
static bool send_command_dispatch_pump(uint8_t *message, uint8_t pump, uint8_t *liters, enum type_fuel_t type);

/**
 * @brief Send command to restart pump
 *
 * @param[in]	message	Buffer where message will be build.
 * @param[in]	pump	Number pump to restart.
 *
 * @return Return true if was success or false in otherwise.
 */
static bool send_command_start_pump(uint8_t *message, uint8_t pump);

/**
 * @brief Callback with functionality of pumps.
 *
 * @param[in]	argument	Argumment passed to task.
 */
static void pump_controller_task(void *argument);


/* ----------------- Implementation private methods ---------------- */
static bool validate_type_fuel(uint8_t *string, enum type_fuel_t type) {
	switch(type) {
	case REGULAR: {
		if (!strcmp((char *)string, "Regular")) {
			return true;
		}
	}
	break;

	case PREMIUM: {
		if (!strcmp((char *)string, "Premium")) {
			return true;
		}
	}
	break;

	case REGULAR_DIESEL: {
		if (!strcmp((char *)string, "Regular diesel")) {
			return true;
		}
	}
	break;

	case PREMIUM_DIESEL: {
		if (!strcmp((char *)string, "Premium diesel")) {
			return true;
		}
	}
	break;

	default: {
		return false;
	}
	break;

	}

	return false;
}

static bool get_status_parameters_pump(uint8_t *root_content, enum type_fuel_t type)
{
	uint16_t index, content_size;
	uint8_t *element, tag[50], content[50];

	if (root_content == NULL) {
		return false;
	}

	index = 0;
	while ((element = xml_get_element_by_index(root_content, index)) == NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		xml_get_element_tag(element, tag);
		content_size = xml_get_element_content(element, content);

		// Get type of fuel and validate if that I would like.
		if (!strcmp((char *)tag, "TYPE_FUEL")) {
			if (!validate_type_fuel(content, type)) {
				return false;
			}
			else {
				status_pump.type_fuel = type;
			}
		}

		if (!strcmp((char *)tag, "LITERS_TANKS")) {
			strncpy((char *)status_pump.fuel_tank, (char *)content, content_size);
		}

		if (!strcmp((char *)tag, "LITERS_FILLED")) {
			strncpy((char *)status_pump.fuel_filled, (char *)content, content_size);
		}

		if (!strcmp((char *)tag, "STATUS_PUMP")) {
			if (!strcmp((char *)content, "filling")) {
				status_pump.filling = true;
			}
			else {
				status_pump.filling = false;
			}
		}

		index++;
	}

	return true;
}

static bool get_attribute_status_tag(uint8_t *element)
{
	uint16_t size_name = 0, size_value = 0;
	uint8_t index = 0, name[50], value[50];

	index = 0;
	memset(name, 0, sizeof(name));
	memset(value, 0, sizeof(value));

	while (xml_get_element_attribute_by_index(element, index, name, &size_name, value, &size_value)) {
		// Ask if response was different of OK, if is then retunr ERROR
		if (!strcmp((char *)name, "response") && !strcmp((char *)value, "OK")) {
			return true;
		}

		memset(name, 0, sizeof(name));
		memset(value, 0, sizeof(value));
		index++;
	}

	return false;
}

static state_pump_t receive_response_state_pump(uint8_t *message, uint8_t pump, enum type_fuel_t type)
{
	uint32_t index;
	uint16_t content_size;
	uint8_t *xml, *content_element_root, *element, tag[50], content[50];

	xml = content_element_root = element = NULL;

	// Function to receive data from UART
	if (pump_controller_uart_receive(message, SIZE_MAX_BUFFER_COMMAND, TIMEOUT_RESPONSE) == 0) {
		return PUMP_ERROR;
	}

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>STATUS</COMMAN_ID>
	 *  	<NUMBER_PUMP>[Number pump]</NUMBER_PUMP>
	 *  	<STATUS response="[Response status]" >
	 *  		<TYPE_FUEL>[Fuel type]</TYPE_FUEL>
	 *  		<LITERS_TANKS>[Quantity of liters on tanks]</LITERS_TANKS>
	 *  		<LITERS_FILLED>[Quantity of liters filled]</LITERS_FILLED>
	 *  		<STATUS_PUMP>[Status of pump]</STATUS_PUMP>
	 *  	</STATUS>
	 *  </CONTROLLER_CEM>
	 *
	 */

	index = 0;
	xml = xml_skip_declaration(message);										// Skip declaration of XML
	content_element_root = xml_get_element_content_ptr(xml, &content_size);		// Get pointers to root of XML

	if (content_element_root == NULL) {
		return PUMP_ERROR;
	}

	// Get pointer of each element from XML
	while((element = xml_get_element_by_index(content_element_root, index)) != NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		xml_get_element_tag(element, tag);

		// Ask for value of COMMAN_ID tag
		if (!strcmp((char *)tag, "COMMAN_ID")) {
			content_size = xml_get_element_content(element, content);
			if (strcmp((char *)content, "STATUS")) {
				return PUMP_ERROR;
			}
		}

		// Ask for value of NUMBER_PUMP tag
		if (!strcmp((char *)tag, "NUMBER_PUMP")) {
			content_size = xml_get_element_content(element, content);
			if (atoi((char *)content) != pump) {
				return PUMP_ERROR;
			}
		}

		if (!strcmp((char *)tag, "STATUS_PUMP")) {

			if (!get_attribute_status_tag(element)) {
				return PUMP_ERROR;
			}

			// Navigate for element of STATUS_PUMPS
			if (!get_status_parameters_pump(element, type)) {
				return PUMP_ERROR;
			}
		}

		index++;
	}

	return status_pump.filling ? PUMP_DISPACHING : PUMP_FINISH_CHARGE;

}

static state_pump_t receive_response_stop_pump(uint8_t *message, uint8_t pump)
{
	uint32_t index;
	uint16_t content_size;
	uint8_t *xml, *content_element_root, *element, tag[50], content[50];

	xml = content_element_root = element = NULL;
	memset(tag, 0, sizeof(tag));
	memset(content, 0, sizeof(content));

	// Function to receive data from UART
	if (pump_controller_uart_receive(message, SIZE_MAX_BUFFER_COMMAND, TIMEOUT_RESPONSE) == 0) {
		return PUMP_ERROR;
	}

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>STOP</COMMAN_ID>
	 *  	<NUMBER_PUMP>[Number pump]</NUMBER_PUMP>
	 *  	<RESPONSE>[Response]</RESPONSE>
	 *  </CONTROLLER_CEM>
	 *
	 */

	index = 0;
	xml = xml_skip_declaration(message);										// Skip declaration of XML
	content_element_root = xml_get_element_content_ptr(xml, &content_size);		// Get pointers to root of XML

	if (content_element_root == NULL) {
		return PUMP_ERROR;
	}

	// Get pointer of each element from XML
	while((element = xml_get_element_by_index(content_element_root, index)) != NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		xml_get_element_tag(element, tag);
		xml_get_element_by_tag(element, tag);
		content_size = xml_get_element_content(element, content);

		if(!strcmp((char *)tag, "COMMAN_ID")) {
			// Ask if is response to stop command
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

static state_pump_t receive_response_dispatch_pump(uint8_t *message, uint8_t pump, enum type_fuel_t type)
{
	uint32_t index, index_attribute;
	uint16_t content_size = 0, size_attribute_name = 0, size_attribute_value = 0;
	uint8_t *xml, *content_element_root, *element, tag[50], content[50], attribute_name[50], attribute_value[50];

	xml = content_element_root = element = NULL;
	memset(tag, 0, sizeof(tag));
	memset(content, 0, sizeof(content));
	memset(attribute_name, 0, sizeof(attribute_name));
	memset(attribute_value, 0, sizeof(attribute_value));

	// Function to receive data from UART
	if (pump_controller_uart_receive(message, SIZE_MAX_BUFFER_COMMAND, TIMEOUT_RESPONSE) == 0) {
		return PUMP_ERROR;
	}

	/* Format of XML
	 *
	 *  <?xml version="1.0" encoding="UTF-8"?>
	 *  <CONTROLLER_CEM>
	 *  	<COMMAN_ID>DISPATCH</COMMAN_ID>
	 *  	<NUMBER_PUMP>[Number pump]</NUMBER_PUMP>
	 *  	<RESPONSE type="type_fuel">[Response]</RESPONSE>
	 *  </CONTROLLER_CEM>
	 *
	 */

	index = 0;
	xml = xml_skip_declaration(message);										// Skip declaration of XML
	content_element_root = xml_get_element_content_ptr(xml, &content_size);		// Get pointers to root of XML

	if (content_element_root == NULL) {
		return PUMP_ERROR;
	}

	// Get pointer of each element from XML
	while ((element = xml_get_element_by_index(content_element_root, index)) != NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		xml_get_element_tag(element, tag);
		content_size = xml_get_element_content(element, content);

		if (!strcmp((char *)tag, "COMMAN_ID")) {
			// Ask if is response to dispache command
			if (strcmp((char *)content, "DISPATCH")) {
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
					if (!validate_type_fuel(attribute_value, type)) {
						return PUMP_ERROR;
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

static state_pump_t receive_response_start_pump(uint8_t *message, uint8_t pump)
{
	uint32_t index;
	uint16_t content_size;
	uint8_t *xml, *content_element_root, *element, tag[50], content[50];

	// Function to receive data from UART
	if (pump_controller_uart_receive(message, SIZE_MAX_BUFFER_COMMAND, TIMEOUT_RESPONSE) == 0) {
		return PUMP_ERROR;
	}

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

	index = 0;
	xml = xml_skip_declaration(message);										// Skip declaration of XML
	content_element_root = xml_get_element_content_ptr(xml, &content_size);		// Get pointers to root of XML

	if (content_element_root == NULL) {
		return PUMP_ERROR;
	}

	// Get pointer of each element from XML
	while ((element = xml_get_element_by_index(content_element_root, index)) != NULL) {
		memset(tag, 0, sizeof(tag));
		memset(content, 0, sizeof(content));
		xml_get_element_tag(element, tag);
		content_size = xml_get_element_content(element, content);

		if (!strcmp((char *)tag, "COMMAN_ID")) {
			// Ask if is response to reset command
			if (strcmp((char *)content, "START")) {
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

static bool send_command_state_pump(uint8_t *message, uint8_t pump)
{
	bool retVal;
	uint8_t number[4];
	uint32_t length;
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
	length = BuildXML_Format(root, message);

	// Sent XML to pump controller.
	retVal = pump_controller_uart_sent(message, length);

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);

	return retVal;
}

static bool send_command_stop_pump(uint8_t *message, uint8_t pump)
{
	bool retVal;
	uint8_t number[4];
	uint32_t length = 0;
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
	length = BuildXML_Format(root, message);

	// Sent XML to pump controller.
	retVal = pump_controller_uart_sent(message, length);

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);

	return retVal;
}

static bool send_command_dispatch_pump(uint8_t *message, uint8_t pump, uint8_t *liters, enum type_fuel_t type)
{
	bool retVal;
	uint8_t number[4];
	uint32_t length;
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
	 *  	<COMMAN_ID>DISPATCH</COMMAN_ID>
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
	switch (type) {
	case REGULAR: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Regular");
	}
	break;

	case PREMIUM: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Premium");
	}
	break;

	case REGULAR_DIESEL: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Regular_diesel");
	}
	break;

	case PREMIUM_DIESEL: {
		BuildXML_AddAtrribute(children_3, (const uint8_t *)"Type", (const uint8_t *)"Premium_diesel");
	}
	break;
	}

	// Set bodies
	BuildXML_AddBody(children_1, (const uint8_t *)"DISPATCH");
	BuildXML_AddBody(children_2, (const uint8_t *)itoa(pump, (char *)number, 10));
	BuildXML_AddBody(children_3, liters);

	// Set up children into root XML
	BuildXML_AddChildren(root, children_1);
	BuildXML_AddChildren(root, children_2);
	BuildXML_AddChildren(root, children_3);

	// Build XML inside string
	BuildXML_FormatHeader(header, message);
	length = BuildXML_Format(root, message);

	// Sent XML to pump controller.
	retVal = pump_controller_uart_sent(message, length);

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);

	return retVal;
}

static bool send_command_start_pump(uint8_t *message, uint8_t pump)
{
	bool retVal;
	uint8_t number[4];
	uint32_t length = 0;
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
	 *  	<COMMAN_ID>START</COMMAN_ID>
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
	BuildXML_AddBody(children_1, (const uint8_t *)"START");
	BuildXML_AddBody(children_2, (const uint8_t *)itoa(pump, (char *)number, 10));

	// Set up children into root XML
	BuildXML_AddChildren(root, children_1);
	BuildXML_AddChildren(root, children_2);

	// Build XML inside string
	BuildXML_FormatHeader(header, message);
	length = BuildXML_Format(root, message);

	// Sent XML to pump controller.
	retVal = pump_controller_uart_sent(message, length);

	// Free memory allocated
	BuildXML_FreeHeader(header);
	BuildXML_Free(root);

	return retVal;
}

static void pump_controller_task(void *argument)
{
	uint8_t msg;
	uint8_t message[SIZE_MAX_BUFFER_COMMAND];
	state_pump_t state;

	for(;;) {

		osMessageQueueGet(controller_pump_queueHandle, &msg, 0L, osWaitForever);

		memset(message, 0, sizeof(message));

		switch(msg) {

		case RESTART_PUMP: {
			send_command_start_pump(message, number_pump);
			memset(message, 0, sizeof(message));
			if (receive_response_start_pump(message, number_pump) == PUMP_OK) {
				// Show message of success
			}
			else {
				// show message of fail
			}

		}
		break;

		case DISPACHE_PUMP: {
			send_command_dispatch_pump(message, number_pump, liters_fuel, type_fuel);
			memset(message, 0, sizeof(message));
			if (receive_response_dispatch_pump(message, number_pump, type_fuel) == PUMP_OK) {
				// Show message of success
			}
			else {
				// show message of fail
			}
		}
		break;

		case STOP_PUMP: {
			send_command_stop_pump(message, number_pump);
			memset(message, 0, sizeof(message));
			if (receive_response_stop_pump(message, number_pump) == PUMP_OK) {
				// Show message of succes
			}
			else {
				// Show message of fail
			}
		}
		break;

		case STATE_PUMP: {
			memset(&status_pump, 0, sizeof(struct status_pump_s));
			send_command_state_pump(message, number_pump);
			state = receive_response_state_pump(message, number_pump, type_fuel);
			if (state == PUMP_ERROR || state == PUMP_FINISH_CHARGE) {
				// Do stop of FreeRTOS timer
			}
			else {
				// Update GUI
			}
		}
		break;

		}
	}
}

/* ----------------- Implementation public methods ----------------- */
bool_t module_pump_controller_started(void)
{
	mutex_new_msg_pump_controller_handle = osMutexNew(&mutex_new_msg_pump_controller_attributes);
	if (mutex_new_msg_pump_controller_handle == NULL) {
		return FALSE;
	}

	/* Create the queue(s) */
	/* creation of controller_pump_queue */
	controller_pump_queueHandle = osMessageQueueNew (2, sizeof(uint8_t), &controller_pump_queue_attributes);

	if (controller_pump_queueHandle == NULL) {
		return FALSE;
	}

	/* creation of Controller_pump */
	Controller_pumpHandle = osThreadNew(pump_controller_task, NULL, &Controller_pump_attributes);

	if (Controller_pumpHandle == NULL) {
		return FALSE;
	}

	return TRUE;
}
