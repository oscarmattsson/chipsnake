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

#define FOOD_REGULAR_WIDTH 2
#define FOOD_REGULAR_HEIGHT 2

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

/* Initialize game logic */
void game_init(void) {
    int i, j;
    for(i=0; i < 32; i++){
        for(j=0; j < 128; j++){
            gamefield[i][j] = game_background[i*128 + j];
        }
    }
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
}

/* Update program logic */
void game_update(int* buttons, int* switches) {

}

/* Draw game */
void game_draw(void) {
    display_full_bin(gamefield);
}
