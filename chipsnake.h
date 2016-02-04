/* basedecl.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson
   Modified 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING */

/* Declare functions from func.c */
void display_image(int x, const uint8_t *data);
void display_image_r(int x, const uint8_t *data);
void display_full(const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
char * itoaconv(int num);
void quicksleep(int cyc);

/* Declare functions from game.c */
void init(void);
void run(void);

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Freeze the program for a set amount of milliseconds */
void delay(int);
/* Written as part of i/o lab: getbtns, getsw */
int getbtns(void);
int getsw(void);
