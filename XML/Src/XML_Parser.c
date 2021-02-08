#include <stdbool.h>
#include <string.h>

#include "Library.h"
#include "XML_Parser.h"

#define TRACE

typedef struct {
	uint8_t  *element;
	uint16_t element_size;
	uint8_t  *tag_name;
	uint16_t tag_name_size;
	uint8_t  *first_attribute;
	uint16_t start_tag_size;
	uint8_t  start_tag_type;
	uint8_t  *content;
	uint16_t content_size;
	uint8_t  *end_tag;
	uint16_t end_tag_size;
} xml_t;

enum {
	XML_TAG_TYPE_INVALID,
	XML_TAG_TYPE_START,
	XML_TAG_TYPE_END,
	XML_TAG_TYPE_EMPTY,
};

/* ---------------------- Prototype private methods ---------------------- */
static bool xml_is_whitespace_char(uint8_t chr);
static bool xml_is_tag_valid_char(uint8_t chr);
static bool xml_is_attribute_valid_char(uint8_t chr);
static bool xml_is_attribute_value_valid_char(uint8_t chr);
static uint8_t *xml_skip_whitespaces(uint8_t *xml);
static bool xml_parse_attribute(uint8_t *xml, uint16_t *size, uint8_t **name, uint16_t *name_size, uint8_t **value, uint16_t *value_size);
static uint8_t xml_parse_element_tag(uint8_t *xml, uint16_t *tag_size, uint8_t **tag_name, uint16_t *tag_name_size, uint8_t **first_attribute);
static bool xml_parse_element(uint8_t *xml, xml_t *out);
static uint8_t *xml_parse_jump_next_element(xml_t *xml_parse);
static bool xml_parse_compare_element_tag(xml_t *xml_parse, uint8_t *tag);

/* -------------------- Implementation private methods ------------------- */
static bool xml_is_whitespace_char(uint8_t chr)
{
	switch (chr) {
	case ' ':
	case '\n':
	case '\r':
	case '\t': {
		return true;
	}
	break;

	default:
		return false;
	}
}

static bool xml_is_tag_valid_char(uint8_t chr)
{
	if (is_alphabetic(chr)) {
		return true;
	}

	if (is_numeric(chr)) {
		return true;
	}

	if (chr == '-') {
		return true;
	}

	if (chr == '_') {
		return true;
	}

	if (chr == '.') {
		return true;
	}

	if (chr == ':') {
		return true;
	}

	return false;
}

static bool xml_is_attribute_valid_char(uint8_t chr)
{
	//TODO
	if (is_alphabetic(chr)) {
		return true;
	}

	if (is_numeric(chr)) {
		return true;
	}

	if (chr == '-') {
		return true;
	}

	if (chr == '_') {
		return true;
	}

	if (chr == '.') {
		return true;
	}

	if (chr == ':') {
		return true;
	}

	return false;
}

static bool xml_is_attribute_value_valid_char(uint8_t chr)
{
	//TODO
	if (is_alphabetic(chr)) {
		return true;
	}

	if (is_numeric(chr)) {
		return true;
	}

	if (chr == '-') {
		return true;
	}

	if (chr == '_') {
		return true;
	}

	if (chr == '.') {
		return true;
	}

	if (chr == ':') {
		return true;
	}

	if (chr == '/') {
		return true;
	}

	if (chr == '@') {
		return true;
	}

	if (chr == '$') {
		return true;
	}

	return false;
}

/*
 * only used when not inside content area
 */
static uint8_t *xml_skip_whitespaces(uint8_t *xml)
{
	uint8_t *new_xml = NULL;

	if (xml) {
		while (*xml && xml_is_whitespace_char(*xml)) {
			xml++;
		}

		/* only if the char isn't a string termination */
		if (*xml) {
			new_xml = xml;
		}
	}

	return new_xml;
}

static bool xml_parse_attribute(uint8_t *xml, uint16_t *size, uint8_t **name, uint16_t *name_size, uint8_t **value, uint16_t *value_size)
{
	uint16_t size_aux = 0, name_size_aux = 0, value_size_aux = 0;
	uint8_t *name_aux, *value_aux;

	/* skip whitespaces counting chars */
	while (*xml && xml_is_whitespace_char(*xml)) {
		xml++;
		size_aux++;
	}
	if (xml == NULL) {
		return false;
	}

	/* after tag name and whitespaces, an attribute must follow */
	if (!xml_is_attribute_valid_char(*xml)) {
		return false;
	}

	/* attribute name pointer */
	name_aux = xml;

	/* loop on attribute name */
	while (xml && *xml && xml_is_attribute_valid_char(*xml)) {
		xml++;
		name_size_aux++;
		size_aux++;
	}
	if (xml == NULL || *xml == 0) {
		return false;
	}

	/* skip whitespaces counting chars */
	while (xml && *xml && xml_is_whitespace_char(*xml)) {
		xml++;
		size_aux++;
	}
	if (xml == NULL || *xml == 0) {
		return false;
	}

	/* after attribute and whitespaces, a '=' must follow */
	if (*xml != '=') {
		return false;
	}
	xml++;
	size_aux++;

	/* skip whitespaces counting chars */
	while (*xml && xml_is_whitespace_char(*xml)) {
		xml++;
		size_aux++;
	}
	if (xml == NULL) {
		return false;
	}

	/* after '=' and whitespaces, a '"' must follow */
	if (*xml != '"') {
		return false;
	}
	xml++;
	size_aux++;

	/* attribute value pointer */
	value_aux = xml;

	/* loop on attribute value */
	while (*xml && xml_is_attribute_value_valid_char(*xml)) {
		xml++;
		value_size_aux++;
		size_aux++;
	}

	/* after attribute value a '"' must follow */
	if (*xml != '"') {
		return false;
	}
	xml++;
	size_aux++;

	/* out values */
	if (size) *size = size_aux;
	if (name) *name = name_aux;
	if (name_size) *name_size = name_size_aux;
	if (value) *value = value_aux;
	if (value_size) *value_size = value_size_aux;

	return true;
}

static uint8_t xml_parse_element_tag(uint8_t *xml, uint16_t *tag_size, uint8_t **tag_name, uint16_t *tag_name_size, uint8_t **first_attribute)
{
	uint8_t tag_type = XML_TAG_TYPE_INVALID;
	uint16_t tag_size_aux = 0, tag_name_size_aux = 0, attribute_size;
	uint8_t tag[4] = "", *tag_name_aux, *first_attribute_aux = NULL;

	/* skip whitespaces */
	xml = xml_skip_whitespaces(xml);
	if (xml == NULL) {
		return XML_TAG_TYPE_INVALID;
	}

	if (*xml != '<') {
		return XML_TAG_TYPE_INVALID;
	}
	xml++;
	tag_size_aux++;

	/* check if it is a start or end tag */
	if (*xml == '/') {
		tag_type = XML_TAG_TYPE_END;
		xml++;
		tag_size_aux++;
	} else {
		tag_type = XML_TAG_TYPE_START;
	}

	/* tag must start right after the '< and start with letter or underscore */
	if (!is_alphabetic(*xml) && *xml != '_') {
		return XML_TAG_TYPE_INVALID;
	}

	/* tag name pointer */
	tag_name_aux = xml;
	tag_name_size_aux++;

	/* tag name aux buf */
	str_append(tag, *xml);

	xml++;
	tag_size_aux++;

	/* validate tag */
	while (*xml && xml_is_tag_valid_char(*xml)) {
		if (strlen((char *)tag) < 3) {
			str_append(tag, *xml);
		}
		if (strlen((char *)tag) == 3) {
			stolower(tag);
			if (!strcmp((char *)tag, "xml")) {
				return XML_TAG_TYPE_INVALID;
			}
		}

		xml++;
		tag_name_size_aux++;
		tag_size_aux++;
	}

	/* valid tag name must be followed by whitespace or closing markup */
	if ((!xml_is_whitespace_char(*xml) && *xml != '>') || xml == NULL || *xml == 0) {
		return XML_TAG_TYPE_INVALID;
	}

	/* loop-check attributes and tag closing */
	while (xml && *xml) {
		/* skip whitespaces counting chars */
		while (xml && *xml && xml_is_whitespace_char(*xml)) {
			xml++;
			tag_size_aux++;
		}
		if (xml == NULL || *xml == 0) {
			return XML_TAG_TYPE_INVALID;
		}

		/* check if tag is closing */
		if (*xml == '>' || (*xml == '/' && xml[1] == '>')) {
			if (*xml == '/') {
				tag_size_aux++;
				tag_type = XML_TAG_TYPE_EMPTY;
			}
			tag_size_aux++;

			/* out values */
			if (tag_size) *tag_size = tag_size_aux;
			if (tag_name) *tag_name = tag_name_aux;
			if (tag_name_size) *tag_name_size = tag_name_size_aux;
			if (first_attribute) *first_attribute = first_attribute_aux;

			return tag_type;
		}

		/* validate attribute */
		if (!xml_parse_attribute(xml, &attribute_size, NULL, NULL, NULL, NULL)) {
			return XML_TAG_TYPE_INVALID;
		}
		if (first_attribute_aux == NULL) first_attribute_aux = xml;
		xml += attribute_size;
		tag_size_aux += attribute_size;
	}

	return XML_TAG_TYPE_INVALID;
}

static bool xml_parse_element(uint8_t *xml, xml_t *out)
{
	xml_t xml_parse;
	uint8_t end_tag[20] = "";

	/* skip whitespaces */
	xml = xml_skip_whitespaces(xml);
	if (xml == NULL) {
		return false;
	}

	xml_parse.element = xml;
	xml_parse.tag_name = NULL;
	xml_parse.tag_name_size = 0;
	xml_parse.first_attribute = NULL;
	xml_parse.start_tag_size = 0;
	xml_parse.start_tag_type = XML_TAG_TYPE_INVALID;
	xml_parse.content = NULL;
	xml_parse.content_size = 0;
	xml_parse.end_tag = NULL;
	xml_parse.end_tag_size = 0;

	/* parse start tag */
	xml_parse.start_tag_type = xml_parse_element_tag(xml, &xml_parse.start_tag_size, &xml_parse.tag_name, &xml_parse.tag_name_size, &xml_parse.first_attribute);

	if (xml_parse.start_tag_type == XML_TAG_TYPE_INVALID) {
		return false;
	}

	if (xml_parse.start_tag_type == XML_TAG_TYPE_END) {
		return false;
	}

	/* end tag */
	strcpy((char *)end_tag, "</");
	strncat((char *)end_tag, (char *)xml_parse.tag_name, xml_parse.tag_name_size);

	/* content parse */
	if (xml_parse.start_tag_type == XML_TAG_TYPE_START) {
		/* element content position */
		xml_parse.content = xml + xml_parse.start_tag_size;

		/* find validate end tag */
		xml_parse.end_tag = (uint8_t *)strstr((char *)xml, (char *)end_tag);
		if (xml_parse_element_tag(xml_parse.end_tag, &xml_parse.end_tag_size, NULL, NULL, NULL) != XML_TAG_TYPE_END) {
			return false;
		}

		/* content size */
		xml_parse.content_size = xml_parse.end_tag - xml_parse.content;
	}

	/* element size */
	xml_parse.element_size = xml_parse.start_tag_size + xml_parse.content_size + xml_parse.end_tag_size;

	/* copy parse results */
	if (out) {
		*out = xml_parse;
	}

	return true;
}

static uint8_t *xml_parse_jump_next_element(xml_t *xml_parse)
{
	uint8_t *xml = NULL;

	if (xml_parse) {
		xml = xml_parse->element + xml_parse->element_size;
	}

	return xml;
}

static bool xml_parse_compare_element_tag(xml_t *xml_parse, uint8_t *tag)
{
	if (xml_parse == NULL) {
		return false;
	}

	if (strncmp((char *)xml_parse->tag_name, (char *)tag, strlen((char *)tag)) != 0) {
		return false;
	}

	return true;
}

/* -------------------- Implementation public methods -------------------- */
uint8_t *xml_skip_declaration(uint8_t *xml)
{
	uint8_t *xml_aux = xml_skip_whitespaces(xml);

	if (xml_aux == NULL) {
		return NULL;
	}

	if (xml_aux[0] == '<' && xml_aux[1] == '?') {
		xml_aux = (uint8_t *)strstr((char *)xml_aux, "?>");
		if (xml_aux == NULL) {
			return NULL;
		}

		xml = xml_aux + 2;
	}

	return xml;
}

uint8_t *xml_get_element_content_ptr_by_tag(uint8_t *xml, uint8_t *tag, uint16_t *size)
{
	xml = xml_get_element_by_tag(xml, tag);

	return xml_get_element_content_ptr(xml, size);
}

uint16_t xml_get_element_content_by_tag(uint8_t *xml, uint8_t *tag, uint8_t *out)
{
	xml = xml_get_element_by_tag(xml, tag);

	return xml_get_element_content(xml, out);
}

uint8_t *xml_get_element_content_ptr(uint8_t *xml, uint16_t *size)
{
	xml_t xml_parse;
	uint8_t *ptr = NULL;
	uint16_t size_aux = 0;

	if (xml_parse_element(xml, &xml_parse) == true) {
		ptr = xml_parse.content;
		size_aux = xml_parse.content_size;
	}

	if (size) {
		*size = size_aux;
	}

	return ptr;
}

uint16_t xml_get_element_content(uint8_t *xml, uint8_t *out)
{
	uint16_t size;
	uint8_t *ptr = xml_get_element_content_ptr(xml, &size);

	if (out) {
		strncpy((char *)out, (char *)ptr, size);
		out[size] = 0;
	}

	return size;
}

uint16_t xml_get_element_tag(uint8_t *xml, uint8_t *out)
{
	xml_t xml_parse;

	if (!xml_parse_element(xml, &xml_parse)) {
		return 0;
	}

	if (out) {
		strncpy((char *)out, (char *)xml_parse.tag_name, xml_parse.tag_name_size);
		out[xml_parse.tag_name_size] = 0;
	}

	return xml_parse.tag_name_size;
}

uint8_t *xml_get_element_by_tag(uint8_t *xml, uint8_t *tag)
{
	xml_t xml_parse;
	uint8_t *element = NULL;

	/* skip spaces */
	xml = xml_skip_whitespaces(xml);

	if (xml) {
		while (xml_parse_element(xml, &xml_parse)) {
			if (xml_parse_compare_element_tag(&xml_parse, tag)) {
				element = xml;
				break;
			} else {
				xml = xml_parse_jump_next_element(&xml_parse);
			}
		}
	}

	return element;
}

uint8_t *xml_get_element_by_index(uint8_t *xml, uint16_t index)
{
	xml_t xml_parse;
	uint8_t *element = NULL;
	uint8_t aux_index = 0;

	/* skip whitespaces before xml element */
	xml = xml_skip_whitespaces(xml);

	if (xml == NULL) {
		return NULL;
	}

	while (xml_parse_element(xml, &xml_parse)) {
		if (aux_index == index) {
			element = xml;
			break;
		}

		aux_index++;
		xml = xml_parse_jump_next_element(&xml_parse);
	}

	return element;
}

uint16_t xml_get_element_attribute_by_index(uint8_t *xml, uint8_t index, uint8_t *name_out, uint16_t *name_size_out, uint8_t *value_out, uint16_t *value_size_out)
{
	xml_t xml_parse;
	uint8_t *attribute_name, *attribute_value;
	uint8_t index_aux = 0;
	uint16_t attribute_size, attribute_name_size, attribute_value_size;

	if (!xml_parse_element(xml, &xml_parse)) {
		return 0;
	}

	if (xml_parse.first_attribute == NULL) {
		return 0;
	}

	xml = xml_parse.first_attribute;

	while (xml_parse_attribute(xml, &attribute_size, &attribute_name, &attribute_name_size, &attribute_value, &attribute_value_size)) {
		if (index_aux == index) {
			if (name_out) {
				strncpy((char *)name_out, (char *)attribute_name, attribute_name_size);
				name_out[attribute_name_size] = 0;
			}
			if (name_size_out) *name_size_out = attribute_name_size;

			if (value_out) {
				strncpy((char *)value_out, (char *)attribute_value, attribute_value_size);
				value_out[attribute_value_size] = 0;
			}
			if (value_size_out) *value_size_out = attribute_value_size;

			return attribute_size;
		}

		index_aux++;
		xml += attribute_size;
	}

	return 0;
}
