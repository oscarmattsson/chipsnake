/* intro.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

#define CENTER 128 / 2

/* Initialize intro logic */
void intro_init(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set up intro screen
  char title[] = "CHIPSNAKE";
  insert_string(CENTER - (sizeof(title)*6) / 2, 12, title, menufield, 1);
  insert_object(0, 4, 24, 20, intro_snake, menufield, 1);
  insert_object(98, 11, 20, 28, intro_mouse, menufield, 1);
}

/* Update intro logic */
void intro_update(int* buttons, int* switches) {
  insert_string(CENTER - (sizeof("Right")*6) / 2, 19, "Right", menufield, 1);
  int x = CENTER - (sizeof("Switch")*6) / 2 - 6;
  x += insert_string(x, 25, "Switch", menufield, 1);
  insert_object(x, 25, 5, 5, arrow_up, menufield, 1);
}

/* Draw game */
void intro_draw(void) {
  display_full_bin(menufield);
}
