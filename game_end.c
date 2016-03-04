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

char names[11][4];
int scores[11];
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
  int tpoints = points;
  end_score[5] = 0; // NUL char
  end_score[4] = (tpoints % 10) + 48;
  tpoints = tpoints / 10;
  end_score[3] = (tpoints % 10) + 48;
  tpoints = tpoints / 10;
  end_score[2] = (tpoints % 10) + 48;
  tpoints = tpoints / 10;
  end_score[1] = (tpoints % 10) + 48;
  tpoints = tpoints / 10;
  end_score[0] = (tpoints % 10) + 48;

  // Set new score as top score
  scores[0] = points;

  int i;
  for(i = 1; i < 11; i++) {
    names[i][3] = 0; // NUL char
    i2c_start();
    i2c_send(EEPROM_WRITE);
    i2c_send(EEPROM_MEM_ADD >> 2);
    i2c_send(EEPROM_MEM_ADD + i*8);
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
      scores[position] = scores[i];
      scores[i] = points;
      position = i;
    }
  }

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

  insert_string(CENTER - (sizeof("GAME OVER")*6) / 2, 3, "GAME OVER", menufield, 1);
  if(position < 10)
    insert_string(67, 13, end_score, menufield, 1);
  else
    insert_string(54, 13, end_score, menufield, 1);
  insert_num(1, 13, scores[1], menufield, 1);
  insert_num(1, 19, points, menufield, 1);


  // Set bottom bar
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
  }

  // Save and go to highscore
  if(switches[0]) {

    // Go to menu
    gamestate = MENU;
  }

  if(position < 10) {
    // Clear letters row
    insert_square(0, 9, 7, 60, 0, menufield);

    if(charpos == 0) {
      insert_square(37, 12, 7, 7, 1, menufield);
      insert_char(38, 13, name[0], menufield, 0);
      insert_char(46, 13, name[1], menufield, 1);
      insert_char(54, 13, name[2], menufield, 1);
    }
    else if(charpos == 1) {
      insert_square(45, 12, 7, 7, 1, menufield);
      insert_char(38, 13, name[0], menufield, 1);
      insert_char(46, 13, name[1], menufield, 0);
      insert_char(54, 13, name[2], menufield, 1);
    }
    else if(charpos == 2) {
      insert_square(53, 12, 7, 7, 1, menufield);
      insert_char(38, 13, name[0], menufield, 1);
      insert_char(46, 13, name[1], menufield, 1);
      insert_char(54, 13, name[2], menufield, 0);
    }
  }
}


/* Draw game */
void game_end_draw(void) {
  display_full_bin(menufield);
}
