#ifndef __XML_PARSER_H__
#define __XML_PARSER_H__

#include "stm32f7xx_hal.h"

uint8_t *xml_skip_declaration(uint8_t *xml);
uint8_t *xml_get_element_content_ptr_by_tag(uint8_t *xml, uint8_t *tag, uint16_t *size);
uint16_t xml_get_element_content_by_tag(uint8_t *xml, uint8_t *tag, uint8_t *out);
uint8_t *xml_get_element_content_ptr(uint8_t *xml, uint16_t *size);
uint16_t xml_get_element_content(uint8_t *xml, uint8_t *out);
uint16_t xml_get_element_tag(uint8_t *xml, uint8_t *out);
uint8_t *xml_get_element_by_tag(uint8_t *xml, uint8_t *key);
uint8_t *xml_get_element_by_index(uint8_t *xml, uint16_t index);
uint16_t xml_get_element_attribute_by_index(uint8_t *xml, uint8_t index, uint8_t *name_out, uint16_t *name_size_out, uint8_t *value_out, uint16_t *value_size_out);

#endif /* __XML_PARSER_H__ */
