/* settings.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

#define NUM_SETTINGS_ITEMS

int settingsselection = 0;
int settingsselected = 0;


/* Initialize game logic */
void settings_init(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set top and bottom bars
  insert_square(0, 0, 7, 128, 1, menufield);
  insert_square(0, 24, 7, 128, 1, menufield);
  insert_string(1, 1, "settings", menufield, 0);
  insert_string(4, 25, "left", menufield, 0);
  insert_string(33, 25, "right", menufield, 0);
  insert_string(71, 25, "sel", menufield, 0);
  insert_string(100, 25, "back", menufield, 0);
  insert_object(3, 13, 5, 3, button_left, menufield, 1);
  insert_object(124, 13, 5, 3, button_right, menufield, 1);

}

/* Update program logic */
void settings_update(int* buttons, int* switches) {
  if(prevgamestate == SETTINGS) {
    if(buttons[0]) {
      if(settingsselected)
        settingsselected = 0;
      else
        gamestate = MENU;
    }
    if(buttons[1]) {
      if(!settingsselected)
        settingsselected = 1;
    }
    if(buttons[2]) {

    }
    if(buttons[3]) {

    }
  }
}

/* Draw game */
void settings_draw(void) {
  display_full_bin(menufield);
}
