/* basefunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson
	 modified 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

/* Declare a helper function which is local to this file */
static void num32asc( char * s, int );

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

#define I2C_EEPROM_READ 0xA1;
#define I2C_EEPROM_WRITE 0xA0;

#define I2C_TEMPERATURE_READ 0x91;
#define I2C_TEMPERATURE_WRITE 0x90;

/* quicksleep:
		A simple function to create a small delay.
		Very inefficient use of computing resources,
		but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/* display_debug
		A function to help debugging.

		After calling display_debug,
		the two middle lines of the display show
		an address and its current contents.

		There's one parameter: the address to read and display.

		Note: When you use this function, you should comment out any
		repeated calls to display_image; display_image overwrites
		about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr )
{
  display_string( 1, "Addr" );
  display_string( 2, "Data" );
  num32asc( &textbuffer[1][6], (int) addr );
  num32asc( &textbuffer[2][6], *addr );
  display_update();
}

/* spi_send_recv
		Send or recieve data from the SPI2 buffer.
*/

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

/* display_init
		Initiate the OLED display as instructed in the
		ChipKit Basic I/O Shield manual.
*/
void display_init(void) {
  DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void i2c_start(void) {
	I2C1CONSET = PIC32_I2CCON_SEN;
}

void i2c_restart(void) {
	I2C1CONSET = PIC32_I2CCON_RSEN;
}

void i2c_stop(void) {
	I2C1CONSET = PIC32_I2CCON_PEN;
}

void i2c_sendbyte(uint8_t data) {
	I2C1TRN = data;
}

void eeprom_mode_write(void) {
	I2C1TRN = I2C_EEPROM_WRITE;
}

void eeprom_mode_read(void) {
	I2C1TRN = I2C_EEPROM_READ;
}

void temperature_mode_write(void) {
	I2C1TRN = I2C_TEMPERATURE_WRITE;
}

void temperature_mode_read(void) {
	I2C1TRN = I2C_TEMPERATURE_READ;
}

/* display_string
		Add a string to the text buffer to be
		drawn at the specified row on the screen.
*/
void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;

	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

/* display_image
		Display a 32x32 pixel image on the screen.

		x is the offset in pixels from the left,
		rounded down to the nearest multiple of 16.
*/
void display_image(int x, const uint8_t *data) {
	int i, j;

	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 32; j++) {
			spi_send_recv(~data[i*32 + j]);
		}
	}
}

/* display_image_r
		Display a mirrored 32x32 pixel image on the screen.

		x is the offset in pixels from the left,
		rounded down to the nearest multiple of 16.
*/
void display_image_r(int x, const uint8_t *data) {
	int i, j;

	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 31; j >= 0; j--) {
			spi_send_recv(~data[i*32 + j]);
		}
	}
}

/* display_full
		Display a 128x32 pixel image on the screen.
*/
void display_full(const uint8_t *data) {
	int i, j, k;

	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x00);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(k = 0; k < 4; k++) {
			for(j = 0; j < 128; j++) {
				spi_send_recv(~data[i*128 + j]);
			}
		}
	}
}

/* display_update
		Clear the screen and draw text from the buffer
		on the screen.
*/
void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n )
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}
