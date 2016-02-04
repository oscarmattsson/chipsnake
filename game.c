/* basework.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   PORTF & (1 << 1) = Button 1
   PORTD & (1 << 5) = Button 2
   PORTD & (1 << 6) = Button 3
   PORTD & (1 << 7) = Button 4

   PORTD & (1 << 8) = Switch 1
   PORTD & (1 << 9) = Switch 2
   PORTD & (1 << 10) = Switch 3
   PORTD & (1 << 11) = Switch 4

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

/* Interrupt Service Routine */
void user_isr(void) {
  return;
}

/* Initialize program logic */
void init(void) {
  return;
}

/* Update program logic */
void run(void) {
  delay(100);
  display_update();
}
