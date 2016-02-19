/* basefunc.c
   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

#define I2C_EEPROM_READ 0xA1;
#define I2C_EEPROM_WRITE 0xA0;

#define I2C_TEMPERATURE_READ 0x91;
#define I2C_TEMPERATURE_WRITE 0x90;

void i2c_start(void) {
	I2C1CONSET = PIC32_I2CCON_SEN;
}

void i2c_restart(void) {
	I2C1CONSET = PIC32_I2CCON_RSEN;
}

void i2c_stop(void) {
	I2C1CONSET = PIC32_I2CCON_PEN;
}

void i2c_sendbyte(uint8_t data) {
	I2C1TRN = data;
}

void eeprom_mode_write(void) {
	I2C1TRN = I2C_EEPROM_WRITE;
}

void eeprom_mode_read(void) {
	I2C1TRN = I2C_EEPROM_READ;
}

void temperature_mode_write(void) {
	I2C1TRN = I2C_TEMPERATURE_WRITE;
}

void temperature_mode_read(void) {
	I2C1TRN = I2C_TEMPERATURE_READ;
}
