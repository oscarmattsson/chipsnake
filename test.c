/* test.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h" /* Declarations of I2C-specific addresses */

#define EEPROM_WRITE 0xA0 // 1010 000 (0)
#define EEPROM_READ 0xA1  // 1010 000 (1)

int invert = 0;

/* Initialize game logic */
void test_init(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

}

/* Update program logic */
void test_update(int* buttons, int* switches) {
  if(buttons[3]) {
    i2c_start();
  }
  if(buttons[2]) {
    i2c_send(EEPROM_WRITE);
  }
  if(buttons[1]) {
  }
  if(buttons[0]) {
    i2c_stop();
  }

  if(switches[2])
    mifinterrupt = sifinterrupt = bifinterrupt = 0;

  if(switches[3])
    invert = 1;
  else
    invert = 0;
}

/* Draw game */
void test_draw(void) {
  insert_square(0, 0, 32, 128, invert, menufield);
  int x;
  x = insert_string(1, 1, "MIF", menufield, 1);
  insert_num(26, 1, mifinterrupt, menufield, 1);
  x = insert_string(1, 7, "SIF", menufield, 1);
  insert_num(26, 7, sifinterrupt, menufield, 1);
  x = insert_string(1, 13, "BIF", menufield, 1);
  insert_num(26, 13, bifinterrupt, menufield, 1);
  x = insert_string(1, 20, "P", menufield, 1);
  insert_num(26, 20, (I2C1STAT & PIC32_I2CSTAT_P) > 0, menufield, 1);
  x = insert_string(1, 26, "S", menufield, 1);
  insert_num(26, 26, (I2C1STAT & PIC32_I2CSTAT_S) > 0, menufield, 1);

  x = insert_string(42, 1, "ADDR", menufield, 1);
  insert_num(68, 1, I2C1ADD, menufield, 1);
  x = insert_string(42, 7, "DATA", menufield, 1);
  insert_num(68, 7, I2C1RCV, menufield, 1);
  // x = insert_string(42, 13, "TBF", menufield, 1);
  // insert_num(68, 13, (I2C1STAT & PIC32_I2CSTAT_TBF) > 0, menufield, 1);
  // x = insert_string(42, 20, "RBF", menufield, 1);
  // insert_num(68, 20, (I2C1STAT & PIC32_I2CSTAT_RBF) > 0, menufield, 1);
  // x = insert_string(42, 26, "WCL", menufield, 1);
  // insert_num(68, 26, (I2C1STAT & PIC32_I2CSTAT_ACKSTAT) > 0, menufield, 1);

  // x = insert_string(83, 1, "DA", menufield, 1);
  // insert_num(109, 1, (I2C1STAT & PIC32_I2CSTAT_DA) > 0, menufield, 1);
  // x = insert_string(83, 7, "SEN", menufield, 1);
  // insert_num(109, 7, (I2C1CON & PIC32_I2CCON_SEN) > 0, menufield, 1);
  // x = insert_string(83, 13, "PEN", menufield, 1);
  // insert_num(109, 13, (I2C1CON & PIC32_I2CCON_PEN) > 0, menufield, 1);
  x = insert_string(83, 20, "BRG", menufield, 1);
  insert_num(109, 20, (I2C1BRG), menufield, 1);
  x = insert_string(83, 26, "ON", menufield, 1);
  insert_num(109, 26, (I2C1CON & PIC32_I2CCON_ON) > 0, menufield, 1);
  display_full_bin(menufield);
}
