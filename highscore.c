/* highscore.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

/* Initialize game logic */
void highscore_init(void) {

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
  insert_string(1, 1, "highscore", menufield, 0);
  insert_string(4, 25, "left", menufield, 0);
  insert_string(33, 25, "right", menufield, 0);
  insert_string(71, 25, "sel", menufield, 0);
  insert_string(100, 25, "back", menufield, 0);
  //insert_string(97, 1, "play", menufield, 0);
  //insert_object(122, 1, 5, 5, arrow_down, menufield, 0);
  insert_object(3, 13, 5, 3, arrow_left, menufield, 1);
  insert_object(124, 13, 5, 3, arrow_right, menufield, 1);

}

/* Update program logic */
void highscore_update(int* buttons, int* switches) {
  if(buttons[0])
    gamestate = MENU;
}

/* Draw game */
void highscore_draw(void) {
  display_full_bin(menufield);
}
