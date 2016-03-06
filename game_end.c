/* game_end.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

char end_score[6];
char name[4];
int charpos = 0;

char names[HIGHSCORE_ENTRIES + 1][4];
int scores[HIGHSCORE_ENTRIES + 1];
int position = 0;

/* Initialize game logic */
void game_end_init(int points) {

  // Reset game
  game_init();

  // Reset name
  name[0] = 'A';
  name[1] = 'A';
  name[2] = 'A';
  name[3] = 0; // NUL char

  // Set score as string
  int p = points;
  end_score[5] = 0; // NUL char

  int i;
  for(i = 4; i >= 0; i--) {
    end_score[i] = (p % 10) + 48;
    p = p / 10;
  }

  // Set new score as top score
  scores[0] = points;

  for(i = 1; i < HIGHSCORE_ENTRIES + 1; i++) {
    names[i][3] = 0; // NUL char
    i2c_start();
    i2c_send(EEPROM_WRITE);
    i2c_send(EEPROM_MEM_ADD >> 2);
    i2c_send(EEPROM_MEM_ADD + (i-1)*HIGHSCORE_LENGTH);
    i2c_restart();
    i2c_send(EEPROM_READ);
    names[i][0] = i2c_recv();
    i2c_ack();
    names[i][1] = i2c_recv();
    i2c_ack();
    names[i][2] = i2c_recv();
    i2c_ack();
    scores[i] = (i2c_recv() - 48) * 10000;
    i2c_ack();
    scores[i] += (i2c_recv() - 48) * 1000;
    i2c_ack();
    scores[i] += (i2c_recv() - 48) * 100;
    i2c_ack();
    scores[i] += (i2c_recv() - 48) * 10;
    i2c_ack();
    scores[i] += (i2c_recv() - 48);
    i2c_stop();

    // Decrement new score position if less than existing score
    if(points <= scores[i]) {
      insert_char(122, 0, 'c', menufield, 1);
      scores[position] = scores[i];
      scores[i] = points;
      position = i;
    }
  }

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  if(position < 10) {
    insert_string(CENTER - (sizeof("GAME OVER")*6) / 2, 3, "GAME OVER", menufield, 1);
    insert_string(64, 13, end_score, menufield, 1);

    // Set bottom bar
    insert_square(0, 24, 7, 128, 1, menufield);
    insert_object(14, 25, 5, 3, button_left, menufield, 0);
    insert_object(46, 25, 5, 3, button_right, menufield, 0);
    insert_object(77, 26, 3, 5, button_down, menufield, 0);
    insert_object(109, 26, 3, 5, button_up, menufield, 0);
    insert_string(98, 18, "SAVE", menufield, 1);
    insert_object(123, 18, 5, 5, switch_up, menufield, 1);
  }
  else {
    insert_string(CENTER - (sizeof("GAME OVER")*6) / 2, 5, "GAME OVER", menufield, 1);
    insert_string(55, 15, end_score, menufield, 1);

    // Set bottom bar
    insert_string(98, 25, "MENU", menufield, 1);
    insert_object(123, 25, 5, 5, switch_up, menufield, 1);
  }

}

/* Update program logic */
void game_end_update(int* buttons, int* switches) {
  if(prevgamestate == GAME_END) {
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

    position = (position + 1) % 11;
  }

  // Save and go to highscore
  if(switches[0]) {

    // int numbytes = HIGHSCORE_ENTRIES * HIGHSCORE_LENGTH;
    // int i;
    // for(i = 0; i < HIGHSCORE_ENTRIES; i++) {
    //   i2c_start();
    //   i2c_send(EEPROM_WRITE);
    //   i2c_send(EEPROM_MEM_ADD >> 2);
    //   i2c_send(EEPROM_MEM_ADD + i*HIGHSCORE_LENGTH);
    //   i2c_send()
    // }

    // Go to menu
    gamestate = MENU;
  }

  if(position < 10) {
    // Clear letters row
    insert_square(36, 12, 7, 23, 0, menufield);

    if(charpos == 0) {
      insert_square(36, 12, 7, 7, 1, menufield);
      insert_char(37, 13, name[0], menufield, 0);
      insert_char(45, 13, name[1], menufield, 1);
      insert_char(53, 13, name[2], menufield, 1);
    }
    else if(charpos == 1) {
      insert_square(44, 12, 7, 7, 1, menufield);
      insert_char(37, 13, name[0], menufield, 1);
      insert_char(45, 13, name[1], menufield, 0);
      insert_char(53, 13, name[2], menufield, 1);
    }
    else if(charpos == 2) {
      insert_square(52, 12, 7, 7, 1, menufield);
      insert_char(37, 13, name[0], menufield, 1);
      insert_char(45, 13, name[1], menufield, 1);
      insert_char(53, 13, name[2], menufield, 0);
    }
  }
}


/* Draw game */
void game_end_draw(void) {
  display_full_bin(menufield);
}
