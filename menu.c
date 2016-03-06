/* menu.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

#define NUM_MENU_ITEMS 3

int menuselection = 0;
int confirmbox = 0;

/* Initialize menu logic */
void menu_init(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set top and bottom bars
  insert_square(0, 0, 7, 128, 1, menufield);
  insert_square(0, 24, 7, 128, 1, menufield);
  insert_string(1, 1, "menu", menufield, 0);
  insert_string(4, 25, "left", menufield, 0);
  insert_string(33, 25, "right", menufield, 0);
  insert_string(71, 25, "sel", menufield, 0);
  //insert_string(100, 25, "back", menufield, 0);
  insert_string(97, 1, "play", menufield, 0);
  insert_object(122, 1, 5, 5, switch_down, menufield, 0);
  insert_object(3, 13, 5, 3, button_left, menufield, 1);
  insert_object(124, 13, 5, 3, button_right, menufield, 1);

}

/* Update program logic */
void menu_update(int* buttons, int* switches) {
  // If previous state is game, default to restart game
  if(prevgamestate == GAME)
    menuselection = 2;

  // If previous state is game_end, default to highscore
  if(prevgamestate == GAME_END)
    menuselection = 0;

  if(prevgamestate == MENU) {
    if(buttons[3]) // Navigate left
      menuselection = (menuselection == 0 ? NUM_MENU_ITEMS - 1 : menuselection - 1);
    if(buttons[2]) {    // Right button
      if(confirmbox) {  // Say yes to restart game
        game_init();
        menu_init();
        confirmbox = 0;
      }
      else              // Navigate right
        menuselection = (menuselection + 1) % NUM_MENU_ITEMS;
    }
    if(buttons[1]) { // Select button
      if(menuselection == 0)
        gamestate = HIGHSCORE;    // Go to highscore screen
      else if(menuselection == 1)
        gamestate = SETTINGS;     // Go to settings screen
      else if(menuselection == 2) {
        if(confirmbox) {          // Say no to restart game
          confirmbox = 0;
          menu_init();
        }
        else confirmbox = 1;      // Go to restart game screen
      }
    }
  }
}

/* Draw game */
void menu_draw(void) {

  if(confirmbox) {  // Draw restart game screen
    insert_square(0, 7, 17, 128, 0, menufield);

    insert_string(CENTER - (sizeof("Are you sure")*6) / 2 - 3, 13, "Are you sure?", menufield, 1);

    insert_square(0, 24, 7, 128, 1, menufield);
    insert_string(39, 25, "YES", menufield, 0);
    insert_string(74, 25, "NO", menufield, 0);
  }
  else {
    insert_square(7, 7, 17, 116, 0, menufield);
    switch(menuselection) {
      case 0: // Draw highscore option
        insert_string(CENTER - (sizeof("Highscore")*6) / 2, 13, "Highscore", menufield, 1);
        break;
      case 1: // Draw settings option
        insert_string(CENTER - (sizeof("Settings")*6) / 2, 13, "Settings", menufield, 1);
        break;
      case 2: // Draw restart game option
        insert_string(CENTER - (sizeof("Restart Game")*6) / 2, 13, "Restart Game", menufield, 1);
        break;
    }
  }

  // Draw screen
  display_full_bin(menufield);
}
