/*
 * PN532.c
 *
 *  Created on: Dec 19, 2020
 *      Author: Hanes
 */
#include <string.h>

#include "PN532.h"
#ifdef PN532_RTOS
#include "cmsis_os.h"
#endif

/* ----------------------- Global private variable ---------------------- */
static uint8_t pn532ack[6] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
static uint8_t pn532response_firmwarevers[6] = {0x00, 0x00, 0xFF, 0x06, 0xFA, 0xD5};
static uint8_t pn532_buffer[PN532_BUFFERSIZE];

/* ---------------------- Protortype private method --------------------- */
static void pn532_delay(uint32_t time);
static bool pn532_is_ready(pn532_drivers *driver);
static void pn532_read_data(pn532_drivers *driver, uint8_t *buffer, uint32_t amount);
static bool pn532_read_ack(pn532_drivers *driver);
static bool pn532_wait_ready(pn532_drivers *driver, uint16_t timeout);
static void pn532_write_command(pn532_drivers *driver, uint8_t *cmd, uint16_t cmd_length);
static bool pn532_send_command_check_ack(pn532_drivers *driver, uint8_t *cmd, uint16_t cmd_length, uint16_t timeout);

/* -------------------- Implementation private method ------------------- */
static void pn532_delay(uint32_t time)
{
#ifdef PN532_RTOS
	osDelay(time/portTICK_PERIOD_MS);
#else
	HAL_Delay(time);
#endif
}

static bool pn532_is_ready(pn532_drivers *driver)
{
	return driver->get_irq();
}

static void pn532_read_data(pn532_drivers *driver, uint8_t *buffer, uint32_t amount)
{
	uint32_t i;

	// Enable PN532 and wait 1 mS
	driver->set_select(true);
	pn532_delay(1);

	// Send message to PN532 to request information
	driver->send_byte(PN532_SPI_DATAREAD);

	for (i = 0; i < amount; i++) {
		buffer[i] = driver->get_byte();	// put data in buffer at position i
	}

	// Disable PN532
	driver->set_select(false);
}

static bool pn532_read_ack(pn532_drivers *driver)
{
	uint8_t ackBuffer[6];		// Array to store read message form PN532

	pn532_read_data(driver, ackBuffer, 6);	// Read message form PN532

	// compare message with ACK message syntax and return true when ACK is received.
	return !strncmp((const char *)ackBuffer, (const char *)pn532ack, 6) ? true : false;
}

static bool pn532_wait_ready(pn532_drivers *driver, uint16_t timeout)
{
#ifdef PN532_RTOS

	uint16_t currentTime = 0;
	// For as long as the timeout is reached try to detect ready signal from PN532
	do {
		if (pn532_is_ready(driver)) {
			return true;
		}
		pn532_delay(1);
		currentTime++;
	} while(currentTime < timeout);
#else
	// For as long as the timeout is reached try to detect ready signal from PN532
	do {
		HAL_Delay(1);
		if (pn532_is_ready(driver)) {
			return true;
		}
		timeout--;
	} while (timeout);
#endif

	return false;
}

static void pn532_write_command(pn532_drivers *driver, uint8_t *cmd, uint16_t cmd_length)
{
	uint8_t checksum;	// variable to store checksum
	uint16_t i;

	cmd_length++;	// increment command length for TFI

	// Enable PN532 and wait 1 mS
	driver->set_select(true);
	pn532_delay(1);

	checksum = PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2;

	driver->send_byte(PN532_SPI_DATAWRITE);	// inform SPI port of PN532 that data will be written
	driver->send_byte(PN532_PREAMBLE);		// Send preamble
	driver->send_byte(PN532_STARTCODE1);	// Send start of packet 1/2
	driver->send_byte(PN532_STARTCODE2);	// Send start of packet 2/2
	driver->send_byte(cmd_length);			// Send packet length (LEN)
	driver->send_byte(~cmd_length + 1);		// Send packet length checksum
	driver->send_byte(PN532_HOSTTOPN532);	// Inform PN532 that data direction is from host to PN532 (TFI)

	checksum += PN532_HOSTTOPN532;

	for (i = 0; i < cmd_length-1; i++)
	{
		driver->send_byte(cmd[i]);			// Send data to PN532
		checksum += cmd[i];
	}

	driver->send_byte(~checksum);				// Send checksum (DCS)
	driver->send_byte(PN532_POSTAMBLE);		// Send postamble

	driver->set_select(false);
}

static bool pn532_send_command_check_ack(pn532_drivers *driver, uint8_t *cmd, uint16_t cmd_length, uint16_t timeout)
{

	// write the command
	pn532_write_command(driver, cmd, cmd_length);

	// Wait for chip to say its ready!
	if (!pn532_wait_ready(driver, timeout)) {
		return false;
	}

	// read acknowledge
	if (!pn532_read_ack(driver)) {
		return false;
	}

	// Wait for chip to say its ready!
	if ( !pn532_wait_ready(driver, timeout) ) {
		return false;
	}

	return true;
}

/* -------------------- Implementation public method -------------------- */
uint32_t pn532_get_firmware_version(pn532_drivers *driver)
{
	uint32_t response = 0;
	uint8_t offset = 0;

	pn532_buffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;	// Set buffer position 0 with command

	// Send command with length of 1 byte, when result is false byte is not send.
	if (!pn532_send_command_check_ack(driver, pn532_buffer, 1, 2)) {
		return 0;
	}

	// Data was successfully send now retrieve data read data packet
	pn532_read_data(driver, pn532_buffer, 12);

	// When buffer is not equal firmware version return
	if (strncmp((char *)pn532_buffer, (char *)pn532response_firmwarevers, 6) != 0) {
		return 0;
	}


	/* Shift relevant data from response in to uint32_t,
	 * while SPI is used though conditional operator select
	 * a shift of 6 positions
	 */
	 offset = 6;	 						// Skip a response byte when using I2C to ignore extra data.
	 response = pn532_buffer[offset++];		// Position 6 of pn532_packetbuffer into response
	 response <<= 8;						// Shift response 8 bits left
	 response |= pn532_buffer[offset++];	// Operation OR position 7 of pn532_packetbuffer with response
	 response <<= 8;						// Shift response 8 bits left
	 response |= pn532_buffer[offset++];	// Operation OR position 8 of pn532_packetbuffer with response
	 response <<= 8;						// Shift response 8 bits left
	 response |= pn532_buffer[offset];		// Operation OR position 9 of pn532_packetbuffer with response

	 return response;
}

bool pn532_sam_config(pn532_drivers *driver)
{
	pn532_buffer[0] = PN532_COMMAND_SAMCONFIGURATION;
	pn532_buffer[1] = 0x01;		// Normal Mode
	pn532_buffer[2] = 0x14;		// Timeout 50ms * 20 = 1 second
	pn532_buffer[3] = 0x01;		// P70_IRQ used

	// send message
	if (!pn532_send_command_check_ack(driver, pn532_buffer, 4, 2)) {
		return false;
	}

	// Read data
	pn532_read_data(driver, pn532_buffer, 8);

	return (pn532_buffer[6] == 0x15) ? true : false;
}

bool pn532_set_passive_activation_retries(pn532_drivers *driver, uint8_t max_retries)
{
	pn532_buffer[0] = PN532_COMMAND_RFCONFIGURATION;
	pn532_buffer[1] = 0x05;		// Config item 5 (MaxRetries)
	pn532_buffer[2] = 0xFF;		// MxRtyATR (default = 0xFF)
	pn532_buffer[3] = 0x01;		// MxRtyPSL (default = 0x01)
	pn532_buffer[4] = max_retries;

	return pn532_send_command_check_ack(driver, pn532_buffer, 5, 2);
}

bool pn532_read_passive_target_id(pn532_drivers *driver, const uint8_t card_baudrate, uint8_t *uid, uint8_t *length_uid, uint16_t timeout)
{
	pn532_buffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
	pn532_buffer[1] = 0x01;	// Maximum 1 card at once (We can set a 0x02 as maximum)
	pn532_buffer[2] = card_baudrate;

	// Send command, command length and timeout.
	if (!pn532_send_command_check_ack(driver, pn532_buffer, 3, timeout)) {
		return false; // No cards read
	}

	// Wait for a card to enter the field (only possible with I2C)
	if (!pn532_wait_ready(driver, timeout)) {
		return false;
	}

	// Read data of packet
	pn532_read_data(driver, pn532_buffer, 20);

	/* ISO14443A card response should be in the following format:

	    byte            Description
	    -------------   ------------------------------------------
	    b0..6           Frame header and preamble
	    b7              Tags Found
	    b8              Tag Number (only one used in this example)
	    b9..10          SENS_RES
	    b11             SEL_RES
	    b12             NFCID Length
	    b13..NFCIDLen   NFCID                                      */

	// Test for the number of tags found. Should be 1!
	if (pn532_buffer[7] != 1) {
		return false;	// No tags found, end of read, return false
	}

	/* Card appears to be Mifare Classic */
	*length_uid = pn532_buffer[12];	// set value of NFCID length to uidLength

	// Move UID to packet buffer
	for (uint8_t i = 0; i < pn532_buffer[12]; i++) {
		uid[i] = pn532_buffer[13+i];
	}

	return true; // return success as card is read.
}
