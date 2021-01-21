/*
 * Build_XML.c
 *
 *  Created on: 6 nov. 2019
 *      Author: hanes
 */

#include <stdlib.h>
#include <string.h>

#include "Build_XML.h"

/* --------------------- Prototype public methods -------------------- */
static uint32_t BuildXML_SetTag(xml_element_t *xml_element, uint8_t *out);

/* ------------------ Implementation private methods ----------------- */
static uint32_t BuildXML_SetTag(xml_element_t *xml_element, uint8_t *out)
{
	uint32_t i;

	strcat((char *)out, "<");
	strcat((char *)out, (char *)xml_element->name_tag);
	strcat((char *)out, " ");
	for(i = 0; i < MAX_COUNT_ATTRIBUTE; i++) {
		if( strlen((char *)xml_element->attribute[i].name) && strlen((char *)xml_element->attribute[i].value) ) {
			strcat((char *)out, (char *)xml_element->attribute[i].name);
			strcat((char *)out, "=");
			strcat((char *)out, "\"");
			strcat((char *)out, (char *)xml_element->attribute[i].value);
			strcat((char *)out, "\"");
			strcat((char *)out, " ");
		}
	}
	strcat((char *)out, ">");

	return strlen((char *)out);
}

/* ------------------ Implementation public methods ------------------ */
xml_element_t* BuildXML_Newxml(const uint8_t *name)
{
	uint32_t i;
	xml_element_t *retVal = (xml_element_t *)malloc(sizeof(xml_element_t));

	/* Se inicializa todos los valores del nuevo XML */
	memset(retVal->name_tag, 0, MAX_LENGTH_ATTRIBUTE);
	strcpy((char *)retVal->name_tag, (char *)name);

	memset(retVal->body, 0, 250);

	for(i = 0; i < MAX_COUNT_CHILDREN; i++) {
		retVal->children[i] = NULL;
	}

	for(i = 0; i < MAX_COUNT_ATTRIBUTE; i++) {
		memset(retVal->attribute[i].name, 0, MAX_LENGTH_ATTRIBUTE);
		memset(retVal->attribute[i].value, 0, MAX_LENGTH_ATTRIBUTE);
	}

	return retVal;
}

xml_header_t *BuildXML_NewXmlHeader(void)
{
	uint32_t i;
	xml_header_t *retVal;

	retVal = (xml_header_t *)malloc(sizeof(xml_header_t));

	for(i = 0; i < MAX_COUNT_ATTRIBUTE; i++) {
		memset(retVal->attribute[i].name, 0, MAX_LENGTH_ATTRIBUTE);
		memset(retVal->attribute[i].value, 0, MAX_LENGTH_ATTRIBUTE);
	}

	return retVal;
}

void BuildXML_AddAtrribute(xml_element_t *xml_element, const uint8_t *name, const uint8_t *value)
{
	uint32_t i;

	for(i = 0; i < MAX_COUNT_ATTRIBUTE; i++) {
		if( strlen((char *)xml_element->attribute[i].name) == 0 && strlen((char *)xml_element->attribute[i].value) == 0 ) {
			strcpy((char *)xml_element->attribute[i].name, (char *)name);
			strcpy((char *)xml_element->attribute[i].value, (char *)value);
			return;
		}
	}
}

void BuildXML_AddAttributeHeader(xml_header_t *header, const uint8_t *name, const uint8_t *value)
{
	uint32_t i;

	for(i = 0; i < MAX_COUNT_ATTRIBUTE; i++) {
		if( strlen((char *)header->attribute[i].name) == 0 && strlen((char *)header->attribute[i].value) == 0 ) {
			strcpy((char *)header->attribute[i].name, (char *)name);
			strcpy((char *)header->attribute[i].value, (char *)value);
			return;
		}
	}
}

void BuildXML_AddBody(xml_element_t *xml_element, const uint8_t *body)
{
	strcpy((char *)xml_element->body, (char *)body);
}

void BuildXML_AddChildren(xml_element_t *xml_element,  xml_element_t *xml_children)
{
	uint32_t i;

	for(i = 0; i < MAX_COUNT_CHILDREN; i++) {
		if( xml_element->children[i] == NULL ) {
			xml_element->children[i] = xml_children;
			return;
		}
	}
}

void BuildXML_FreeHeader(xml_header_t *xml_header)
{
	free(xml_header);
	xml_header = NULL;
}

xml_element_t *BuildXML_Free(xml_element_t *xml_element)
{
	uint32_t i;

	if( xml_element == NULL ) {
		return NULL;
	}

	for(i = 0; i < MAX_COUNT_CHILDREN; i++) {
		xml_element->children[i] = BuildXML_Free(xml_element->children[i]);
	}

	free(xml_element);

	return NULL;
}

void BuildXML_FormatHeader(xml_header_t *xml_header, uint8_t *out)
{
	uint32_t i;

	strcat((char *)out, "<?xml ");

	for(i = 0; i < MAX_COUNT_ATTRIBUTE; i++ ) {
		if( strlen((char *)xml_header->attribute[i].name) && strlen((char *)xml_header->attribute[i].value) ) {
			strcat((char *)out, (char *)xml_header->attribute[i].name);
			strcat((char *)out, "=");
			strcat((char *)out, "\"");
			strcat((char *)out, (char *)xml_header->attribute[i].value);
			strcat((char *)out, "\"");
			strcat((char *)out, " ");
		}
	}
	strcat((char *)out, "?>");
}

uint32_t BuildXML_Format(xml_element_t *xml_element, uint8_t *out)
{
	uint32_t i, length = 0;

	if( xml_element == NULL ) {
		return length;
	}

	/* Se arma el el tag con sus datos */
	length += BuildXML_SetTag(xml_element, out);

	if( strlen((char *)xml_element->body) != 0 ) {
		strcat((char *)out, (char *)xml_element->body);
	}
	else {
		for(i = 0; i < MAX_COUNT_CHILDREN; i++ ) {
			length += BuildXML_Format(xml_element->children[i], out+length);
		}
	}

	strcat((char *)out, "</");
	strcat((char *)out, (char *)xml_element->name_tag);
	strcat((char *)out, ">");
	//sprintf(out+length, "</%s>", xml_element->name_tag);

	return strlen((char *)out);
}
