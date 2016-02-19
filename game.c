/* game.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

int timeoutcount = 0;



/* Interrupt Service Routine */
void user_isr(void) {
  if(IFS(0) & (1 << 8)) {
    timeoutcount++;
    IFSCLR(0) = (1 << 8);
  }

  if(timeoutcount >= 10) {
    timeoutcount = 0;

    display_full(snake);
  }
}

/* Initialize program logic */
void init(void) {

  /* Initialize clock */
  T2CON = 0x70;              // Stop timer and set prescaling to 1:256

  TMR2 = 0;                     // Reset counter
  PR2 = (80000000 / 256) / 10;  // Set period to 100ms

  IPCSET(2) = 0x1F;             // Set priority 7 and subpriority 3
  IFSCLR(0) = (1 << 8);         // Clear interrupt flag for timer 2
  IECSET(0) = (1 << 8);         // Enable interrupts for timer 2

  /* Initialize I2C */
  IPCSET(6) = 0x1B00;       // Set priority 6 and subpriority 3
  IFSCLR(0) = 0xE0000000;   // Clear interrupt flags for I2C1MIF, I2C1SIF and I2CBIF
  IECSET(0) = 0xE0000000;   // Enable interrupts for I2C1MIF, I2C1SIF and I2CBIF

  enable_interrupt(); // Enable interrupts globally

  T2CONSET = 0x8000;    // Start clock
  I2C1CONSET = 0x8000;  // Start I2C bus
}

/* Update program logic */
void run(void) {

}
