/* intro.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

#define CENTER 128 / 2

uint8_t introfield[32][128];

/* Initialize intro logic */
void intro_init(void) {

  // Clear screen
  int x, y;
  for(y = 0; y < 32; y++) {
    for(x = 0; x < 128; x++) {
      introfield[y][x] = 0;
    }
  }

  // Set up intro screen
  char title[] = "CHIPSNAKE";
  insert_string(CENTER - (sizeof(title)*6) / 2, 12, title, introfield, 1);
}

/* Update intro logic */
void intro_update(int* buttons, int* switches) {
  insert_string(CENTER - (sizeof("Flip right switch up")*6) / 2, 19, "Flip right switch up", introfield, 1);
}

/* Draw game */
void intro_draw(void) {
  display_full_bin(introfield);
}
