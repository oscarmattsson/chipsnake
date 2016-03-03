/* help.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

#define NUM_HELP_ITEMS 3

int helpselection = 0;
int helpselected = 0;
int helprow = 0;
const int const helprows[NUM_HELP_ITEMS] = { 6, 9, 19 }; // 0: controls, 1: points, 2: rules

char help_controls[][20] = {
  "BTN 1: TURN UP",
  "BTN 2: TURN LEFT",
  "BTN 3: TURN RIGHT",
  "BTN 4: TURN UP",
  "---",
  "SW 4: MENU / GAME",
};

char help_points[][20] = {
  "HIGHER SPEED GIVES",
  "HIGHER SCORE",
  "   - 1p*speed",
  "   - 3p*speed",
  "   - 5p*speed",
  "   - 5p*speed",
  "   - 5p*speed",
  "   - 10p*speed",
  "   - 10p*speed",
};

char help_rules[][20] = {
  "Eat food to earn",
  "points and to grow",
  "longer.",
  "See food types",
  "section for more",
  "info.",
  "---",
  "Edges of the game",
  "field can be passed",
  "through safely.",
  "---",
  "If walls are active",
  "edges will expand",
  "and collision will",
  "cause game over.",
  "---",
  "Collision with your",
  "own tail will cause",
  "game over.",
};

/* Initialize game logic */
void help_init(void) {
}

/* Update program logic */
void help_update(int* buttons, int* switches) {
  if(prevgamestate == HELP) {
    if(buttons[0]) {
      if(helpselected) {  // Exit helpselection
        helpselected = 0;
        helprow = 0;
      }
      else
        gamestate = MENU; // Exit help
    }
    if(buttons[1]) {
      if(!helpselected)   // Enter helpselection
        helpselected = 1;
    }
    if(buttons[2]) {
      // Move down one row in helpselection
      if(helpselected && helprow < helprows[helpselection] - 1)
        helprow++;
      // Move to next helpselection
      else if(!helpselected)
        helpselection = (helpselection + 1) % NUM_HELP_ITEMS;
    }
    if(buttons[3]) {
      // Move up one row in helpselection
      if(helpselected && helprow > 0)
        helprow--;
      // Move to previous helpselection
      else if(!helpselected)
        helpselection = (helpselection - 1) < 0 ? NUM_HELP_ITEMS - 1 : helpselection - 1;
    }
  }
}

/* Draw game */
void help_draw(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set top and bottom bars
  insert_square(0, 0, 7, 128, 1, menufield);
  insert_square(0, 24, 7, 128, 1, menufield);

  // Insert back button
  insert_string(100, 25, "back", menufield, 0);

  // Help index
  if(!helpselected) {
    // Insert common elements
    insert_string(1, 1, "help", menufield, 0);
    insert_string(4, 25, "left", menufield, 0);
    insert_string(33, 25, "right", menufield, 0);
    insert_string(71, 25, "sel", menufield, 0);
    insert_object(3, 13, 5, 3, button_left, menufield, 1);
    insert_object(124, 13, 5, 3, button_right, menufield, 1);

    switch(helpselection) {
      case 0:
        insert_string(CENTER - (sizeof("CONTROLS")*6) / 2, 13, "CONTROLS", menufield, 1);
        break;
      case 1:
        insert_string(CENTER - (sizeof("POINTS")*6) / 2, 13, "POINTS", menufield, 1);
        break;
      case 2:
        insert_string(CENTER - (sizeof("RULES")*6) / 2, 13, "RULES", menufield, 1);
        break;
    }
  }
  // Help sections
  else {
    // Insert common elements
    if(helprow > 0) {
      insert_string(10, 25, "up", menufield, 0);
      insert_object(3, 11, 3, 5, button_up, menufield, 1);
    }
    if(helprow < helprows[helpselection]) {
      insert_string(36, 25, "down", menufield, 0);
      insert_object(3, 17, 3, 5, button_down, menufield, 1);
    }

    int i; // Loop variable

    switch(helpselection) {
      case 0: // Draw info about controls
        insert_string(1, 1, "CONTROLS", menufield, 0);
        insert_string(11, 9, help_controls[helprow], menufield, 1);
        if(helprow + 1 < helprows[0])
          insert_string(11, 17, help_controls[helprow + 1], menufield, 1);
        break;
      case 1: // Draw info about food and points
        insert_string(1, 1, "FOOD TYPES", menufield, 0);
        insert_string(11, 9, help_points[helprow], menufield, 1);
        if(helprow + 1 < helprows[1])
          insert_string(11, 17, help_points[helprow + 1], menufield, 1);
        switch(helprow) { // Draw food icons with matching points
          case 1:
            insert_object(20, 18, 3, 3, food_regular, menufield, 0);
            break;
          case 2:
            insert_object(20, 10, 3, 3, food_regular, menufield, 0);
            insert_object(15, 18, 4, 8, food_turtle, menufield, 0);
            break;
          case 3:
            insert_object(15, 10, 4, 8, food_turtle, menufield, 0);
            insert_object(15, 18, 4, 8, food_frog, menufield, 0);
            break;
          case 4:
            insert_object(15, 10, 4, 8, food_frog, menufield, 0);
            insert_object(15, 18, 4, 8, food_spider, menufield, 0);
            break;
          case 5:
            insert_object(15, 10, 4, 8, food_spider, menufield, 0);
            insert_object(17, 17, 5, 6, food_bird, menufield, 0);
            break;
          case 6:
            insert_object(17, 9, 5, 6, food_bird, menufield, 0);
            insert_object(16, 18, 3, 7, food_mouse, menufield, 0);
            break;
          case 7:
            insert_object(16, 10, 3, 7, food_mouse, menufield, 0);
            insert_object(16, 18, 3, 7, food_lizard, menufield, 0);
            break;
          case 8:
            insert_object(16, 10, 3, 7, food_lizard, menufield, 0);
            break;
        }
        break;
      case 2: // Draw info about rules
        insert_string(1, 1, "RULES", menufield, 0);
        insert_string(11, 9, help_rules[helprow], menufield, 1);
        if(helprow + 1 < helprows[2])
          insert_string(11, 17, help_rules[helprow + 1], menufield, 1);
        break;
    }
  }

  // Draw screen
  display_full_bin(menufield);
}
