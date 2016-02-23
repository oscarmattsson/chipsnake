/* highscore.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

/* Initialize game logic */
void highscore_init(void) {

}

/* Update program logic */
void highscore_update(void) {

}

/* Draw game */
void highscore_draw(void) {
  display_string(0, "Highscore Screen");
  display_update();
}
