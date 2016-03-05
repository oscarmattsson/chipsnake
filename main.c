/* basemain.c

   This file written 2016 by Oscar Mattsson
   Some original code written 2015 by Axel Isaksson and F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "i2c-defs.h"

#define INTRO_TIME 0

state gamestate = INTRO;
state prevgamestate = INTRO;
int speed = 1;

int mifinterrupt = 0;
int sifinterrupt = 0;
int bifinterrupt = 0;

unsigned seed;

uint8_t menufield[32][128];

int timeoutcount = 0;
int introcount = 0;

int buttons[4] = { 0, 0, 0, 0 };
int switches[4] = { 0, 0, 0, 0 };

/* Update state of program */
void update(void) {
  state tempgamestate = gamestate;
  switch(gamestate) {
    case TEST:
      if(!switches[1])
        gamestate = MENU;
      if(prevgamestate != TEST)
        test_init();
      test_update(buttons, switches);
      test_draw();
      break;
    case INTRO:
      if(introcount >= INTRO_TIME) {
        if(switches[0])
          gamestate = MENU;
        else
          intro_update(buttons, switches);
      }
      intro_draw();
      break;
    case MENU:
      if(!switches[0])
        gamestate = GAME;
      if(switches[1])
        gamestate = TEST;
      if(prevgamestate != MENU)
        menu_init();
      menu_update(buttons, switches);
      menu_draw();
      break;
    case GAME:
      if(switches[0])
        gamestate = MENU;
      if(switches[2]) {
        seed = (unsigned)TMR2;
        game_end_init(seed);
        gamestate = GAME_END;
      }
      if(prevgamestate != GAME)
        game_draw();
      game_update(buttons, switches);
      break;
    case GAME_END:
      game_end_update(buttons, switches);
      game_end_draw();
      break;
    case HIGHSCORE:
      if(prevgamestate != HIGHSCORE)
        highscore_init();
      highscore_update(buttons, switches);
      highscore_draw();
      break;
    case SETTINGS:
      if(prevgamestate != SETTINGS)
        settings_init();
      settings_update(buttons, switches);
      settings_draw();
      break;
    case HELP:
      if(prevgamestate != HELP)
        help_init();
      help_update(buttons, switches);
      help_draw();
      break;
    default:
      break;
  }
  prevgamestate = tempgamestate;
}

/* Interrupt Service Routine */
void user_isr(void) {

  if(IFS(0) & (1 << 8)) {
    timeoutcount++;
    IFSCLR(0) = (1 << 8);
  }

  if(timeoutcount >= 10 - speed) {
    timeoutcount = 0;

    if(gamestate == TEST) {
      test_draw();
    }

    if(gamestate == INTRO) {
      if(introcount < INTRO_TIME)
        introcount++;
      else
        update();
    }
    else if(gamestate == GAME) {
      if(!switches[3]) {
        while(!game_move());
      }
      game_draw();
    }
  }

  /* Handle I2C interrupts */
  // if(IFS(0) & (1 << 31)) {
  //   mifinterrupt = 1;
  //   IFSCLR(0) = 1 << 31;
  // }
  // if(IFS(0) & (1 << 30)) {
  //   sifinterrupt = 1;
  //   IFSCLR(0) = 1 << 30;
  // }
  // if(IFS(0) & (1 << 29)) {
  //   bifinterrupt = 1;
  //   IFSCLR(0) = 1 << 29;
  // }
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

  /* Set up clock */
  /* Initialize clock */
  T2CON = 0x70;              // Stop timer and set prescaling to 1:256

  TMR2 = 0;                     // Reset counter
  PR2 = (80000000 / 256) / 10;  // Set period to 100ms

  IPCSET(2) = 0x1F;             // Set priority 7 and subpriority 3
  IFSCLR(0) = (1 << 8);         // Clear interrupt flag for timer 2
  IECSET(0) = (1 << 8);         // Enable interrupts for timer 2

  /* Initialize I2C */
  I2C1CON = 0x0;            // Clear control register
	I2C1BRG = 0x0C2;          // Set Baud Generator Divisor
	I2C1STAT = 0x0;           // Clear status register
	I2C1CONSET = 1 << 13;     // SIDL = 1
  // IPCSET(6) = 0x1B00;       // Set priority 6 and subpriority 3
  // IFSCLR(0) = 0xE0000000;   // Clear interrupt flags for I2C1MIF, I2C1SIF and I2CBIF
  // IECSET(0) = 0xE0000000;   // Enable interrupts for I2C1MIF, I2C1SIF and I2CBIF

  enable_interrupt(); // Enable interrupts globally

  T2CONSET = 0x8000;    // Start clock
  I2C1CONSET = 0x8000;  // Start I2C bus

  /* Program initialization */
  game_init();
  intro_init();

  update();

  while(1) {

    // Perform an update if inputs or gamestate are changed
    if(buttons[0] != (PORTF & 1 << 1) ||
       buttons[1] != (PORTD & 1 << 5) ||
       buttons[2] != (PORTD & 1 << 6) ||
       buttons[3] != (PORTD & 1 << 7) ||
       switches[0] != (PORTD & 1 << 8) ||
       switches[1] != (PORTD & 1 << 9) ||
       switches[2] != (PORTD & 1 << 10) ||
       switches[3] != (PORTD & 1 << 11) ||
       prevgamestate != gamestate) {

      buttons[0] = PORTF & 1 << 1;
      buttons[1] = PORTD & 1 << 5;
      buttons[2] = PORTD & 1 << 6;
      buttons[3] = PORTD & 1 << 7;
      switches[0] = PORTD & 1 << 8;
      switches[1] = PORTD & 1 << 9;
      switches[2] = PORTD & 1 << 10;
      switches[3] = PORTD & 1 << 11;

      update();
    }
  }
}
