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
uint8_t gamefieldconv[512];

/* Initialize game logic */
void game_init(void) {

    int i, j, k, n;

    for(i=0; i < 32; i++){
      for(j=0; j < 128; j++){
        gamefield[i][j] = game_background[i*128 + j];
      }
    }


/*
    for(i=0; i < 32; i++){
        for(j=0; j < 128; j++){
            gamefield[i][j] = 0;
        }
    }

*/
    uint8_t decimal = 0;
    uint8_t array[8];
    int m=0;

    for(j=0; j < 4; j++){
        for(n=0; n < 128; n++){
            m = j * 8;
            for(k=0; k < 8; k++){
                if(gamefield[m][n] == 0){
                    array[k] = 0; }
                else{
                    array[k] = 1; }
                m++;

            }

            for(i = 7; i >= 0; i--){
                decimal = decimal * 2 + array[i];
            }

            gamefieldconv[j*128 + n] = decimal;
        }
    }
}

/* Update program logic */
void game_update(void) {

}

/* Draw game */
void game_draw(void) {
    display_full((const uint8_t*)(gamefieldconv));
}
