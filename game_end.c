/* game_end.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

int score;
int charpos = 0;
char name[3];

/* Initialize game logic */
void game_end_init(int points) {

  // Reset game
  game_init();

  // Reset name
  name[0] = 'A';
  name[1] = 'A';
  name[2] = 'A';

  // Set score
  score = points;

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set top and bottom bars
  insert_string(CENTER - (sizeof("GAME OVER")*6) / 2, 3, "GAME OVER", menufield, 1);
  insert_square(CENTER - (4 + 10), 9, 7, 7, 1, menufield);
  insert_char(CENTER - (4 + 9), 10, name[0], menufield, 0);
  insert_char(CENTER - (4 + 0), 10, name[1], menufield, 1);
  insert_char(CENTER - (4 - 8), 10, name[2], menufield, 1);

  insert_square(0, 24, 7, 128, 1, menufield);
  insert_object(14, 25, 5, 3, button_left, menufield, 0);
  insert_object(46, 25, 5, 3, button_right, menufield, 0);
  insert_object(77, 26, 3, 5, button_down, menufield, 0);
  insert_object(109, 26, 3, 5, button_up, menufield, 0);

  insert_string(98, 18, "SAVE", menufield, 1);
  insert_object(123, 18, 5, 5, switch_up, menufield, 1);
}

/* Update program logic */
void game_end_update(int* buttons, int* switches) {

  // Update name
  if(buttons[3]) {
    charpos = charpos == 0 ? 2 : charpos - 1;
  }
  if(buttons[2]) {
    charpos = (charpos + 1) % 3;
  }
  if(buttons[1]) {
    name[charpos] = name[charpos] == 65 ? 90 : name[charpos] - 1;
  }
  if(buttons[0]) {
    name[charpos] = name[charpos] == 90 ? 65 : name[charpos] + 1;
  }

  // Save and go to highscore
  if(switches[0]) {
    // TODO: Write to EEPROM

    // Go to highscore
    gamestate = HIGHSCORE;
  }

  // Clear letters row
  insert_square(0, 9, 7, 128, 0, menufield);

  if(charpos == 0) {
    insert_square(CENTER - (4 + 10), 9, 7, 7, 1, menufield);
    insert_char(CENTER - (4 + 9), 10, name[0], menufield, 0);
    insert_char(CENTER - (4 + 0), 10, name[1], menufield, 1);
    insert_char(CENTER - (4 - 8), 10, name[2], menufield, 1);
  }
  else if(charpos == 1) {
    insert_square(CENTER - (4 + 1), 9, 7, 7, 1, menufield);
    insert_char(CENTER - (4 + 9), 10, name[0], menufield, 1);
    insert_char(CENTER - (4 + 0), 10, name[1], menufield, 0);
    insert_char(CENTER - (4 - 8), 10, name[2], menufield, 1);
  }
  else if(charpos == 2) {
    insert_square(CENTER - (4 - 7), 9, 7, 7, 1, menufield);
    insert_char(CENTER - (4 + 9), 10, name[0], menufield, 1);
    insert_char(CENTER - (4 + 0), 10, name[1], menufield, 1);
    insert_char(CENTER - (4 - 8), 10, name[2], menufield, 0);
  }
}

/* Draw game */
void game_end_draw(void) {
  display_full_bin(menufield);
}
