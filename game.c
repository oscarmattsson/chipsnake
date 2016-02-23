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
    game_draw_figures_r(23, 15, 5, 5, font_min[3], gamefield);
}

/* Update program logic */
void game_update(void) {

}

/* Draw game */
void game_draw(void) {
    display_full_bin((const uint8_t*)(gamefield));
}
