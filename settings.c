/* settings.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

#define NUM_SETTINGS_ITEMS 2

int settingsselection = 0;
int settingsselected = 0;
int settingscolumn = 0;

const int const settingscolumns[NUM_SETTINGS_ITEMS] = { 8, 2 }; // 0: controls, 1: points, 2: rules

/* Initialize game logic */
void settings_init(void) {
  settingscolumn = speed - 1;
  settingsselection = 0;
  settingsselected = 0;
}

/* Update program logic */
void settings_update(int* buttons, int* switches) {
  if(prevgamestate == SETTINGS) {
    if(buttons[0]) {
      if(settingsselected) {          // Exit Settings section
        settingsselected = 0;
        if(settingsselection == 0) {
          speed = settingscolumn + 1; // Update speed value
          int i;
          PORTECLR = 0xFF;            // Clear LEDs
          for(i = 0; i < speed; i++) {
            PORTESET = (1 << 7) >> i; // Update speed representation on LEDs
          }
        }
        else if(settingsselection == 1) {
          walls = settingscolumn;     // Update walls value
        }
        game_init();  // Restart game after settings are changed
      }
      else
        gamestate = MENU;     // Return to menu
    }
    if(buttons[1]) {
      if(!settingsselected)   // Enter Settings section
        settingsselected = 1;
    }
    if(buttons[2]) {
      if(settingsselected)
        settingscolumn = (settingscolumn + 1) % settingscolumns[settingsselection];
      // Move to next settingsselection
      else if(!settingsselected) {
        settingsselection = (settingsselection + 1) % NUM_SETTINGS_ITEMS;
        if(settingsselection == 0)
          settingscolumn = speed - 1;
        else if(settingsselection == 1)
          settingscolumn = walls;
      }
    }
    if(buttons[3]) {
      // Move up one column in settingsselection
      if(settingsselected)
        settingscolumn = (settingscolumn - 1) < 0 ? settingscolumns[settingsselection] - 1 : settingscolumn - 1;
      // Move to previous settingsselection
      else if(!settingsselected) {
        settingsselection = (settingsselection - 1) < 0 ? NUM_SETTINGS_ITEMS - 1 : settingsselection - 1;
          if(settingsselection == 0)
            settingscolumn = speed - 1;
          else if(settingsselection == 1)
            settingscolumn = walls;
      }
    }
  }
}

/* Draw game */
void settings_draw(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set top and bottom bars
  insert_square(0, 0, 7, 128, 1, menufield);
  insert_square(0, 24, 7, 128, 1, menufield);

  // Insert buttons
  insert_string(4, 25, "left", menufield, 0);
  insert_string(33, 25, "right", menufield, 0);

  insert_object(3, 13, 5, 3, button_left, menufield, 1);
  insert_object(124, 13, 5, 3, button_right, menufield, 1);

  // Setting index
  if(!settingsselected){

    insert_string(1, 1, "settings", menufield, 0);
    insert_string(100, 25, "back", menufield, 0);
    insert_string(71, 25, "sel", menufield, 0);

    switch(settingsselection) {
      case 0:
        insert_string(CENTER - (sizeof("SPEED")*6) / 2, 13, "SPEED", menufield, 1);
        break;
      case 1:
        insert_string(CENTER - (sizeof("WALLS")*6) / 2, 13, "WALLS", menufield, 1);
        break;
      }
    }
// Settings sections
  else {
    // Insert common elements
    insert_string(100, 25, "save", menufield, 0);
    int x = 45;
    int y = 14;
    int j;
    switch(settingsselection) {
      case 0: // Draw info about speed
        insert_string(1, 1, "SPEED", menufield, 0);
        for(j=0; j < settingscolumn + 1; j++){
          insert_square(x + j*5, y, 4, 3, 1, menufield);
        }
        break;
      case 1: // Draw info about walls
        insert_string(1, 1, "WALLS", menufield, 0);
        if(settingscolumn == 0)
          insert_string(CENTER - (sizeof("OFF")*6) / 2, 13, "OFF", menufield, 1);
        else if(settingscolumn == 1)
          insert_string(CENTER - (sizeof("ON")*6) / 2, 13, "ON", menufield, 1);
        break;
    }
  }

  // Draw screen
  display_full_bin(menufield);
}
