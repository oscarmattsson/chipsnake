/* game.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */
#define gamefieldleft 1
#define gamefieldtop 1
#define gamefieldwitdh 126
#define gamefieldheight 24

uint8_t gamefield[32][128];

/* Initialize game logic */
void game_init(void) {
    int i, j;
    for(i=0; i < 32; i++){
        for(j=0; j < 128; j++){
            gamefield[i][j] = game_background[i*128 + j];
        }
    }
    /*insert_char(26, 10, 'Z', gamefield, 0);
    insert_num(26, 26, 0001, gamefield, 1);
    int offset = insert_string(2, 2, "Hello 123 world", gamefield, 0);
    insert_string(2 + offset, 2, " or something", gamefield, 0);
    insert_square(8, 8, 3, 50, 1, gamefield);*/
}

/* Update program logic */
void game_update(void) {

}

void game_snake(void){
  // insert_square(int x, int y, int height, int width, int value, uint8_t dest[32][128]);
  // plus är nedåt
  int x = 40, y = 30, height = 1, width = 2, value = 3;
  int i, length;
  length = 3;

  // if move up --------------
  if (value == 2){
    for(i = 0; i <length; i++){
      insert_square(x, y, width, height, value, gamefield);
      insert_square(x+1, y+1, width, height, value, gamefield);
      y = y-4;
    }
    // Head towards up
    insert_square(x, y, width, height, value, gamefield);
    insert_square(x+1, y, 4, height, value, gamefield);
  }
  // if move right -----------
  if (value == 3){
    for(i = 0; i <length; i++){
      insert_square(x, y, height, width, value, gamefield);
      insert_square(x-1, y+1, height, width, value, gamefield);
      x = x+4;
    }
    // Head towards right
    insert_square(x, y, height, width, value, gamefield);
    insert_square(x-1, y+1, height, 4, value, gamefield);
  }
  // if move down -----------
  if (value == 4){
    for(i = 0; i <length; i++){
      insert_square(x, y, width, height, value, gamefield);
      insert_square(x+1, y-1, width, height, value, gamefield);
      y = y+4;
    }
    // Head towards down
    insert_square(x, y, width, height, value, gamefield);
    insert_square(x+1, y-1, 4, height, value, gamefield);
  }

  // if move left ------------
  if (value == 5){
    for(i = 0; i <length; i++){
      insert_square(x, y, height, width, value, gamefield);
      insert_square(x+1, y+1, height, width, value, gamefield);
      x = x-4;
    }
    // Head towards left
    insert_square(x-3, y, height, width, value, gamefield);
    insert_square(x-3, y+1, height, 4, value, gamefield);
  }


}

/* Draw game */
void game_draw(void) {
    game_snake();
    display_full_bin(gamefield);
}
