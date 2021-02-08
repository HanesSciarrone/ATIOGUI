/*
 * Build_XML.h
 *
 *  Created on: 6 nov. 2019
 *      Author: hanes
 */

#ifndef BUILD_XML_H_
#define BUILD_XML_H_

#include "stm32f7xx_hal.h"

#define MAX_LENGTH_ATTRIBUTE	50
#define MAX_COUNT_ATTRIBUTE		15
#define MAX_COUNT_CHILDREN		20

typedef struct
{
	uint8_t name[MAX_LENGTH_ATTRIBUTE];
	uint8_t value[MAX_LENGTH_ATTRIBUTE];
}xml_attribute_t;

typedef struct
{
	xml_attribute_t attribute[MAX_COUNT_ATTRIBUTE];
}xml_header_t;

typedef struct xml_element_s
{
	uint8_t name_tag[MAX_LENGTH_ATTRIBUTE];
	uint8_t body[250];
	xml_attribute_t attribute[MAX_COUNT_ATTRIBUTE];
	struct xml_element_s *children[MAX_COUNT_CHILDREN];
}xml_element_t;

xml_element_t* BuildXML_Newxml(const uint8_t *name);
xml_header_t *BuildXML_NewXmlHeader(void);
void BuildXML_AddAtrribute(xml_element_t *xml_element, const uint8_t *name, const uint8_t *value);
void BuildXML_AddAttributeHeader(xml_header_t *header, const uint8_t *name, const uint8_t *value);
void BuildXML_AddBody(xml_element_t *xml_element, const uint8_t *body);
void BuildXML_AddChildren(xml_element_t *xml_element,  xml_element_t *xml_children);
void BuildXML_FreeHeader(xml_header_t *xml_header);
xml_element_t *BuildXML_Free(xml_element_t *xml_element);
void BuildXML_FormatHeader(xml_header_t *xml_header, uint8_t *out);
uint32_t BuildXML_Format(xml_element_t *xml_element, uint8_t *out);

#endif /* BUILD_XML_H_ */
