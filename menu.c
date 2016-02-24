/* menu.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

uint8_t menufield[32][128];

/* Initialize game logic */
void menu_init(void) {
  int x, y;
  for(y = 0; y < 32; y++) {
    for(x = 0; x < 128; x++) {
      menufield[y][x] = 0;
    }
  }

  insert_square(0, 0, 7, 128, 1, menufield);
}

/* Update program logic */
void menu_update(void) {

}

/* Draw game */
void menu_draw(void) {
  display_full_bin(menufield);
}
