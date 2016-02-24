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

		for(j = 0; j < 128; j++) {
			spi_send_recv(data[i*128 + j]);
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

void display_full_bin(const uint8_t data[32][128]){
	int i, j, k, n;
	uint8_t decimal = 0;
	uint8_t array[8];
	int m=0;
	uint8_t decdata[128*4];

	for(j=0; j < 4; j++){
			for(n=0; n < 128; n++){
					m = j * 8;
					for(k=0; k < 8; k++){
							if(data[m][n] == 0){
									array[k] = 0; }
							else{
									array[k] = 1; }
							m++;

					}
					for(i = 7; i >= 0; i--){
							decimal = decimal * 2 + array[i];
					}
					decdata[j*128 + n] = decimal;
			}
	}
	display_full((const uint8_t*)decdata);
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n )
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

/* Insert an object into a "binary" screen array. */
void insert_object(int x, int y, int height, int width,
	const uint8_t src[], uint8_t dest[32][128], int reverse){
  int i, j;
  for(i = 0; i < height; i++){
    for(j = 0; j < width; j++){
			if(reverse)
				dest[y+i][x+j] = src[i*width + j];
			else
	      dest[y+i][x+j] = src[i*width + j] > 0 ? 0 : 1;
    }
  }
}

/* Insert a character (0-9 or A-Z) into a "binary" screen array.
	 Other chars will be interpreted as a blank space.
	 Returns offset to next character position. */
int insert_char(int x, int y, char c, uint8_t dest[32][128], int reverse) {
	if(x >= 0 && x + 3 < 128) {	// Only draw on this row
		if(c >= 48 && c <= 57) { 	// If c is a number (0-9)
			c -= 47;
			insert_object(x, y, 5, 3, numbers[c], dest, reverse);
			return 4;
		}
	}
	if(x >= 0 && x + 5 <= 128) {	// Only draw on this row
		if(c >= 65) {
			if(c >= 97 && c <= 122)	// If c is a lowercase letter (a-z)
				c -= (97 - 65); 			// Turn lower case into uppercase
			if(c <= 90) { 					// If c is an uppercase letter (A-Z)
				c -= 64;
				insert_object(x, y, 5, 5, letters[c], dest, reverse);
			}
		}
		else {
			insert_object(x, y, 5, 5, letters[0], dest, reverse);
		}
		return 6;
	}
	return 0;
}

/* Insert a string of characters (0-9 or A-Z) into a "binary" screen array.
	 Uses insert_char to insert each character.
	 Returns offset to next character position */
int insert_string(int x, int y, const char* s, uint8_t dest[32][128], int reverse) {
	int offset = 0;
	while(*s != 0 && x + offset < 128) {
		offset += insert_char(x + offset, y, *s, dest, reverse);
		s++;
	}
	return offset;
}

/* Insert a number (0-9) into a "binary" screen array.
	 Larger numbers will be inserted recursively in
	 sequence after eachother.
   Negative numbers will be interpreted as a blank space.
	 Returns offset to next character position. */
int insert_num(int x, int y, int n, uint8_t dest[32][128], int reverse) {
	if(n > 9) { 																							// If n is larger than 9, start recursion
		int offset = insert_num(x, y, n / 10, dest, reverse);	// Recursive call on n / 10
		insert_object(x + offset, y, 5, 3,											// Once returned from recursion, draw lowest digit
			numbers[(n % 10) + 1], dest, reverse);
		return offset + 4; 																			// Increase offset for each digit
	}
	else if(n >= 0) {	// Single digit or end of recursion
		insert_object(x, y, 5, 3, numbers[n + 1], dest, reverse);
		return 4;
	}
	else // Draw single space
		insert_object(x, y, 5, 3, numbers[0], dest, reverse);
	return 4;
}

void insert_square(int x, int y, int width, int height, uint8[32][128], int reverse) {
	
}
