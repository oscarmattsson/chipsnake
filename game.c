/* game.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

#define GAMEFIELD_LEFT 1
#define GAMEFIELD_TOP 1
#define GAMEFIELD_WIDTH 126
#define GAMEFIELD_HEIGHT 24

#define FOOD_REGULAR_WIDTH 3
#define FOOD_REGULAR_HEIGHT 3

#define FOOD_LIZARD_WIDTH 7
#define FOOD_LIZARD_HEIGHT 3

#define FOOD_MOUSE_WIDTH 7
#define FOOD_MOUSE_HEIGHT 3

#define FOOD_BIRD_WIDTH 6
#define FOOD_BIRD_HEIGHT 5

#define FOOD_SPIDER_WIDTH 8
#define FOOD_SPIDER_HEIGHT 4

#define FOOD_FROG_WIDTH 8
#define FOOD_FROG_HEIGHT 4

#define FOOD_TURTLE_WIDTH 8
#define FOOD_TURTLE_HEIGHT 4

uint8_t gamefield[32][128];
int snake[8];  // x,y,value för huvud och svans

/* Initialize game logic */
void game_init(void) {    // ändra loopen
    int i, j;
    for(i=0; i < 32; i++){
        for(j=0; j < 128; j++){
            gamefield[i][j] = game_background[i*128 + j];
        }
    }
    // Initialize the snake
    int x = 40, y = 16, height = 1, width = 2, value = 3, length = 6;
    for(i = 0; i <length; i++){
      insert_square(x, y, height, width, value, gamefield);
      insert_square(x-1, y+1, height, width, value, gamefield);
      x = x+3;
    }
    // Head towards right
    insert_square(x, y, height, width, value, gamefield);
    insert_square(x-1, y+1, height, 3, value, gamefield);

    snake = {58, 16, 3, 40, 16, 3, 3, 0};


/*
    insert_object(1, 12 - (FOOD_REGULAR_HEIGHT / 2),
      FOOD_REGULAR_HEIGHT, FOOD_REGULAR_WIDTH, food_regular, gamefield, 0);
    insert_object(5, 12 - (FOOD_LIZARD_HEIGHT / 2),
      FOOD_LIZARD_HEIGHT, FOOD_LIZARD_WIDTH, food_lizard, gamefield, 0);
    insert_object(15, 12 - (FOOD_MOUSE_HEIGHT / 2),
      FOOD_MOUSE_HEIGHT, FOOD_MOUSE_WIDTH, food_mouse, gamefield, 0);
    insert_object(24, 12 - (FOOD_BIRD_HEIGHT / 2),
      FOOD_BIRD_HEIGHT, FOOD_BIRD_WIDTH, food_bird, gamefield, 0);
    insert_object(32, 12 - (FOOD_SPIDER_HEIGHT / 2),
      FOOD_SPIDER_HEIGHT, FOOD_SPIDER_WIDTH, food_spider, gamefield, 0);
    insert_object(42, 12 - (FOOD_FROG_HEIGHT / 2),
      FOOD_FROG_HEIGHT, FOOD_FROG_WIDTH, food_frog, gamefield, 0);
    insert_object(52, 12 - (FOOD_TURTLE_HEIGHT / 2),
      FOOD_TURTLE_HEIGHT, FOOD_TURTLE_WIDTH, food_turtle, gamefield, 0);
      */
}

/* Update program logic */
void game_update(int* buttons, int* switches, ) {
  // Up-button = 0
  // Right-button = 1
  // Left-button = 2
  // Down-button = 3
  // om turn = 0 --> ingen turn piece
  // om value = 0 --> ingen body piece
  // game_snake(int x, int y, int value, int head_value, int turn, int turn_value) 6 parameterar

  if(buttons[0]){   // svänger uppåt
    game_snake(snake[0], snake[1], 0, 0, 1, 2)
    snake[6] = snake[2];   // kopierar över head_value till gamla head_value
    if(snake[6] == 3){    // om huvudet åkte mot höger innan
      snake[0] += 1;
      snake[1] -= 2;
      snake[2] = 2;
    }
    if(snake[6] == 4){    // om huvudet åkte mot vänster innan
      snake[0] -= 2;
      snake[1] -= 2;           // extrahuvud 1
      snake[2] = 2;
      snake[7] = 1;
    }
  }
  if(buttons[1]]){    // svänger höger
    if(snake[6] == 2){    // om huvudet åkte uppåt innan
      snake[0] += 1;
      snake[1] -= 2;        // extra huvud 2 som ger andra koordinater
      snake[2] = 3;
      snake[7] = 1;
    }
    if(snake[6] == 5){    // om huvudet åkte neråt innan
      snake[0] += 2;
      snake[1] += 2;
      snake[2] = 3;
    }
  }
  if(buttons[2]){    // svänger vänster
    game_snake(snake[0], snake[1], 0, 0, 3, 4)
    if(snake[6] == 2){    // om huvudet åkte uppåt innan
      snake[0] -= 2;
      snake[1] -= 2;        // extra huvud 3
      snake[2] = 4;
      snake[7] = 1;
    }
    if(snake[6] == 5){    // om huvudet åkte neråt innan
      snake[0] -= 2;
      snake[1] += 1;
      snake[2] = 4;        // ---------------------------------
    }
  }
  if(buttons[3]){    // svänger nedåt
    if(snake[6] == 3){    // om huvudet åkte mot höger innan
      snake[0] += 2;
      snake[1] += 2;
      snake[2] = 5;
    }
    if(snake[6] == 4){    // om huvudet åkte mot vänster innan
      snake[0] -= 2;
      snake[1] += 1;     // extrahuvud 4
      snake[2] = 5;
      snake[7] = 1;
    }
  }

}

void game_snake(int x, int y, int value, int head_value, int turn, int turn_value){
  // insert_square(int x, int y, int height, int width, int value, uint8_t dest[32][128]);
  // plus är nedåt
int height = 1, width = 2;

if (value == 2){         // body upp
  insert_square(x, y, width, height, value, gamefield);
  insert_square(x+1, y+1, width, height, value, gamefield);
  }
if (head_value == 2){    // head up
  insert_square(x, y, width, height, head_value, gamefield);
  insert_square(x+1, y, 3, height, head_value, gamefield);
  }

if (value == 3){        // body right
  insert_square(x, y, height, width, value, gamefield);
  insert_square(x-1, y+1, height, width, value, gamefield);
  }
if (head_value == 3){   // head right
  insert_square(x, y, height, width, head_value, gamefield);
  insert_square(x-1, y+1, height, 3, head_value, gamefield);
  }

if (value == 4){        // body left
  insert_square(x, y, height, width, value, gamefield);
  insert_square(x+1, y+1, height, width, value, gamefield);
  }
if (head_value == 4){  // head left
  insert_square(x-3, y, height, width, head_value, gamefield);
  insert_square(x-3, y+1, height, 3, head_value, gamefield);
  }

if (value == 5){        // body down
  insert_square(x, y, width, height, value, gamefield);
  insert_square(x+1, y-1, width, height, value, gamefield);
  }
if (head_value == 5){   // head down
  insert_square(x, y, width, height, head_value, gamefield);
  insert_square(x+1, y-1, 3, height, head_value, gamefield);
  }
/*
if(extra == 1){   // extra head  --------------------
  insert_square(x, y, width+1, height, head_value, gamefield);
  insert_square(x+1, y, width, height, head_value, gamefield); }

if(extra == 2){    // extra head
  insert_square(x, y, height, width+1, head_value, gamefield);
  insert_square(x+1, y+1, height, width, head_value, gamefield); }

if(extra == 3){    // extra head
  insert_square(x, y, height, width+1, head_value, gamefield);
  insert_square(x+1, y+1, height, width, head_value, gamefield); }

if(extra == 4){    // extra head
  insert_square(x, y, width+1, height, head_value, gamefield);
  insert_square(x+1, y+1, width, height, head_value, gamefield); }
*/

if (turn == 1){        // turn piece facing up -----------------------
  insert_square(x, y, height, height, turn_value, gamefield);
  insert_square(x-1, y+1, height, width+1, turn_value, gamefield);
}
if (turn == 2){        // turn piece facing down
  insert_square(x, y, height, width+1, turn_value, gamefield);
  insert_square(x+1, y+1, height, height, turn_value, gamefield);
}
if (turn == 3){        // turn piece facing left
  insert_square(x, y, height, height, turn_value, gamefield);
  insert_square(x+1, y-1, width+1, height, turn_value, gamefield);
}
if (turn == 4){       // turn piece facing right
  insert_square(x, y, width+1, height, turn_value, gamefield);
  insert_square(x+1, y+1, height, height, turn_value, gamefield);
}


}
// The snake moves forward
void game_move(void){


}

/* Draw game */
void game_draw(void) {
    game_snake();
    display_full_bin(gamefield);
}
