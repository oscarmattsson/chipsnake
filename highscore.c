/* highscore.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

int highscore_offset = 0;

/* Initialize game logic */
void highscore_init(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  // Set top and bottom bars
  insert_square(0, 0, 7, 128, 1, menufield);
  insert_square(0, 24, 7, 128, 1, menufield);
  insert_string(1, 1, "highscore", menufield, 0);
  insert_string(4, 25, "left", menufield, 0);
  insert_string(33, 25, "right", menufield, 0);
  insert_string(100, 25, "back", menufield, 0);
  insert_object(3, 13, 5, 3, button_left, menufield, 1);
  insert_object(124, 13, 5, 3, button_right, menufield, 1);

}

/* Update program logic */
void highscore_update(int* buttons, int* switches) {
  if(buttons[0])
    gamestate = MENU;
  if(buttons[2])
    highscore_offset = (highscore_offset + 1) % HIGHSCORE_ENTRIES;
  if(buttons[3])
    highscore_offset = (highscore_offset - 1 < 0) ? HIGHSCORE_ENTRIES - 1 : highscore_offset - 1;
}

/* Draw game */
void highscore_draw(void) {

  // Clear index from screen
  insert_square(105, 1, 5, 30, 1, menufield);

  // Draw index
  insert_num(119, 1, 10, menufield, 0);
  insert_char(113, 1, '/', menufield, 0);
  if(highscore_offset + 1 == 10)
    insert_num(105, 1, 10, menufield, 0);
  else
    insert_num(109, 1, highscore_offset + 1, menufield, 0);

  // Draw highscore entry from index
  char entry[HIGHSCORE_LENGTH + 2];
  entry[HIGHSCORE_LENGTH + 1] = 0; // NUL char
  i2c_start();
  i2c_send(EEPROM_WRITE);
  i2c_send(EEPROM_MEM_ADD >> 2);
  i2c_send(EEPROM_MEM_ADD + highscore_offset * HIGHSCORE_LENGTH);
  i2c_restart();
  i2c_send(EEPROM_READ);
  entry[0] = i2c_recv();
  int i;
  for(i = 1; i < HIGHSCORE_LENGTH + 1; i++) {
    if(i == 3) {
      entry[i] = ' ';
      continue;
    }
    i2c_ack();
    entry[i] = i2c_recv();
  }
  i2c_stop();

  insert_string(CENTER - (4*6 + 5*4) / 2, 13, entry, menufield, 1);

  // Draw screen
  display_full_bin(menufield);
}
