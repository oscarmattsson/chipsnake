/* menu.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

#define NUM_MENU_ITEMS 3

uint8_t menufield[32][128];
int center = 128 / 2;
int menuselection = 0;

/* Initialize menu logic */
void menu_init(void) {

  // Clear screen
  int x, y;
  for(y = 0; y < 32; y++) {
    for(x = 0; x < 128; x++) {
      menufield[y][x] = 0;
    }
  }

  // Set top and bottom bars
  insert_square(0, 0, 7, 128, 1, menufield);
  insert_square(0, 24, 7, 128, 1, menufield);
  insert_string(1, 1, "menu", menufield, 1);
  insert_string(4, 25, "left", menufield, 1);
  insert_string(33, 25, "right", menufield, 1);
  insert_string(71, 25, "sel", menufield, 1);
  insert_string(100, 25, "back", menufield, 1);
  insert_string(97, 1, "play", menufield, 1);
  insert_object(122, 1, 5, 5, arrow_down, menufield, 1);
  insert_object(3, 13, 5, 3, arrow_left, menufield, 0);
  insert_object(124, 13, 5, 3, arrow_right, menufield, 0);

  // Set up menu options
}

/* Update program logic */
void menu_update(int* buttons, int* switches) {
  if(buttons[3]) // Left button
    menuselection = (menuselection == 0 ? NUM_MENU_ITEMS - 1 : menuselection - 1);
  if(buttons[2]) // Right button
    menuselection = (menuselection + 1) % NUM_MENU_ITEMS;
}

/* Draw game */
void menu_draw(void) {
  insert_square(7, 8, 15, 116, 0, menufield);
  switch(menuselection) {
    case 0:
      insert_string(center - (sizeof("Highscore")*6) / 2, 13, "Highscore", menufield, 0);
      break;
    case 1:
      insert_string(center - (sizeof("Settings")*6) / 2, 13, "Settings", menufield, 0);
      break;
    case 2:
      insert_string(center - (sizeof("Help")*6) / 2, 13, "Help", menufield, 0);
      break;
  }
  display_full_bin(menufield);
}
