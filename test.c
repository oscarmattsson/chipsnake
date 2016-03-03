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
#define EEPROM_MEM_ADD 0x0000

int invert = 0;
int ack = 1;
char data[8];
int offset = 0;
int word = 0;
// int scores[10];

/* Initialize game logic */
void test_init(void) {

  // Clear screen
  insert_square(0, 0, 32, 128, 0, menufield);

}

/* Update program logic */
void test_update(int* buttons, int* switches) {
  if(buttons[3]) {
    // for(word = 0; word < 10; word++) {
      i2c_start();
      ack = i2c_send(EEPROM_WRITE) & ack;         // Select EEPROM in write mode
      ack = i2c_send(EEPROM_MEM_ADD >> 2) & ack;  // Set highest address bits
      ack = i2c_send(EEPROM_MEM_ADD + word*8) & ack;       // Set lowest address bits
      ack = i2c_send('A' + word) & ack;         // Write A to address
      ack = i2c_send('B' + word) & ack;         // Write B to address + 1
      ack = i2c_send('C' + word) & ack;        // Write C to address + 2
      ack = i2c_send('0') & ack;                 // Write D to address + 3
      ack = i2c_send('1') & ack;                 // Write E to address + 4
      ack = i2c_send('2') & ack;                 // Write F to address + 5
      ack = i2c_send('3') & ack;                 // Write G to address + 6
      ack = i2c_send('4') & ack;                 // Write G to address + 7
      i2c_stop();
    // }
    word = (word + 1) % 10;
  }
  if(buttons[2]) {
    // int i;
    // for(i = 0; i < 10; i++) {
    //   i2c_start();
    //   i2c_send(EEPROM_WRITE);
    //   i2c_send(EEPROM_MEM_ADD >> 2);
    //   i2c_send(EEPROM_MEM_ADD + i*8 + 3);
    //   i2c_restart();
    //   i2c_send(EEPROM_READ);
    //   scores[i] = (i2c_recv() - 48) * 10000;
    //   i2c_ack();
    //   scores[i] += (i2c_recv() - 48) * 1000;
    //   i2c_ack();
    //   scores[i] += (i2c_recv() - 48) * 100;
    //   i2c_ack();
    //   scores[i] += (i2c_recv() - 48) * 10;
    //   i2c_ack();
    //   scores[i] += (i2c_recv() - 48);
    //   i2c_stop();
    // }
  }
  if(buttons[1]) {
    i2c_start();
    ack = i2c_send(EEPROM_READ) & ack;
    data[offset++] = (char)i2c_recv();
    i2c_stop();
  }
  if(buttons[0]) {
    i2c_start();
    ack = i2c_send(EEPROM_WRITE) & ack;         // Select EEPROM in write mode
    ack = i2c_send(EEPROM_MEM_ADD >> 2) & ack;  // Set highest address bits
    ack = i2c_send(EEPROM_MEM_ADD + word*8) & ack;       // Set lowest address bits
    i2c_restart();
    ack = i2c_send(EEPROM_READ) & ack;         // Select EEPROM in write mode
    data[0] = (char)i2c_recv();
    for(offset = 1; offset < 8; offset++) {
      i2c_ack();
      data[offset] = (char)i2c_recv();
    }
    i2c_stop();
    word = (word + 1) % 10;
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

  x = insert_string(42, 1, "DATA", menufield, 1);
  insert_num(68, 1, data[offset], menufield, 1);
  x = insert_string(42, 7, "STR", menufield, 1);
  insert_string(68, 7, data, menufield, 1);
  x = insert_string(42, 13, "ACK", menufield, 1);
  insert_num(68, 13, ack, menufield, 1);
   x = insert_string(42, 20, "WORD", menufield, 1);
   insert_num(68, 20, word, menufield, 1);
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
  // insert_num(1, 1, scores[0], menufield, 1);
  // insert_num(1, 7, scores[1], menufield, 1);
  // insert_num(1, 13, scores[2], menufield, 1);
  // insert_num(1, 19, scores[3], menufield, 1);
  // insert_num(1, 25, scores[4], menufield, 1);
  //
  // insert_num(64, 1, scores[5], menufield, 1);
  // insert_num(64, 7, scores[6], menufield, 1);
  // insert_num(64, 13, scores[7], menufield, 1);
  // insert_num(64, 19, scores[8], menufield, 1);
  // insert_num(64, 25, scores[9], menufield, 1);

  display_full_bin(menufield);
}
