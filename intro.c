/* intro.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */


/* Initialize game logic */
void intro_init(void) {

}

/* Update program logic */
void intro_update(void) {

}

/* Draw game */
void intro_draw(void) {
  display_string(1, "Intro Screen");
  display_update();
}
