/* settings.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

int row = 0;

/* Initialize game logic */
void settings_init(void) {

}

/* Update program logic */
void settings_update(void) {

}

/* Draw game */
void settings_draw(void) {
  display_string(row, "HIGHSCORE");
  display_update();
}
