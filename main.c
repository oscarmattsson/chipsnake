/* basemain.c

   This file written 2015 by Axel Isaksson,
   modified 2015 by F Lundevall
   modified 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

enum state { INTRO, MENU, GAME, GAME_END, HIGHSCORE, SETTINGS, HELP };
enum state gamestate = INTRO;
enum state prevgamestate = INTRO;

int timeoutcount = 0;
int introcount = 0;

int buttons[4] = { 0, 0, 0, 0 };
int switches[4] = { 0, 0, 0, 0 };

/* Interrupt Service Routine */
void user_isr(void) {
  if(IFS(0) & (1 << 8)) {
    timeoutcount++;
    IFSCLR(0) = (1 << 8);
  }

  if(gamestate == INTRO) {
    if(introcount >= 1) { // Go from intro to menu after 5 seconds
      prevgamestate = INTRO;
      gamestate = MENU;
      switches[0] = -1;
    }
  }
  if(timeoutcount >= 10) {
    timeoutcount = 0;
    if(gamestate == INTRO)
      introcount++;
    else if(gamestate == GAME)
      game_draw();
  }
}

int main(void) {
	/* Set up peripheral bus clock */
  /* OSCCONbits.PBDIV = 1; */
  OSCCONCLR = 0x100000; /* clear PBDIV bit 1 */
	OSCCONSET = 0x080000; /* set PBDIV bit 0 */

	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Set up input pins */
	TRISFSET = 0x2; // Button 1
	TRISDSET = 0xE0; // Buttons 2-4
  TRISDSET = 0xF00; // Switches 1-4

  /* Set up output pins */
  TRISECLR = 0xFF;

	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
  SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	display_init();
  display_update();

  /* Set up clock */
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

  intro_init();
  menu_init();
  game_init();

  while(1) {

    if(gamestate != INTRO) {
      if(PORTD & (1 << 8)) {
        prevgamestate = gamestate;
        gamestate = MENU;
      } else {
        prevgamestate = gamestate;
        gamestate = GAME;
      }
    }

    if(buttons[0] != (PORTF & 1 << 1) ||
       buttons[1] != (PORTD & 1 << 5) ||
       buttons[2] != (PORTD & 1 << 6) ||
       buttons[3] != (PORTD & 1 << 7) ||
       switches[0] != (PORTD & 1 << 8) ||
       switches[1] != (PORTD & 1 << 9) ||
       switches[2] != (PORTD & 1 << 10) ||
       switches[3] != (PORTD & 1 << 11)
       ) {

      buttons[0] = PORTF & 1 << 1;
      buttons[1] = PORTD & 1 << 5;
      buttons[2] = PORTD & 1 << 6;
      buttons[3] = PORTD & 1 << 7;
      switches[0] = PORTD & 1 << 8;
      switches[1] = PORTD & 1 << 9;
      switches[2] = PORTD & 1 << 10;
      switches[3] = PORTD & 1 << 11;

      switch(gamestate) {
        case INTRO:
          intro_update(buttons, switches); // TODO: Remove or implement
          intro_draw();
          break;
        case MENU:
          menu_update(buttons, switches);
          menu_draw();
          break;
        case GAME:
          if(prevgamestate != GAME)
            game_draw();
          game_update(buttons, switches);
          break;
        case GAME_END:
          game_end_update(buttons, switches);
          game_end_draw();
          break;
        case HIGHSCORE:
          highscore_update(buttons, switches);
          highscore_draw();
          break;
        case SETTINGS:
          settings_update(buttons, switches);
          settings_draw();
          break;
        case HELP:
          help_update(buttons, switches);
          help_draw();
          break;
        default:
          break;
      }
    }
  }
}
