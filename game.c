/* game.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

uint8_t gamefield[128][32];

/* Initialize game logic */
void game_init(void) {

}

/* Update program logic */
void game_update(void) {

}

/* Draw game */
void game_draw(void) {
  display_string(1, "Game Screen");
  display_update();
}
