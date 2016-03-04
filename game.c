/* game.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */

#define GAMEFIELD_LEFT 1
#define GAMEFIELD_TOP 1
#define GAMEFIELD_WIDTH 126
#define GAMEFIELD_HEIGHT 24

#define FOOD_REGULAR_WIDTH 3
#define FOOD_REGULAR_HEIGHT 3
#define FOOD_REGULAR_VALUE 1

#define FOOD_LIZARD_WIDTH 7
#define FOOD_LIZARD_HEIGHT 3
#define FOOD_LIZARD_VALUE 10

#define FOOD_MOUSE_WIDTH 7
#define FOOD_MOUSE_HEIGHT 3
#define FOOD_MOUSE_VALUE 10

#define FOOD_BIRD_WIDTH 6
#define FOOD_BIRD_HEIGHT 5
#define FOOD_BIRD_VALUE 5

#define FOOD_SPIDER_WIDTH 8
#define FOOD_SPIDER_HEIGHT 4
#define FOOD_SPIDER_VALUE 5

#define FOOD_FROG_WIDTH 8
#define FOOD_FROG_HEIGHT 4
#define FOOD_FROG_VALUE 5

#define FOOD_TURTLE_WIDTH 8
#define FOOD_TURTLE_HEIGHT 4
#define FOOD_TURTLE_VALUE 3

uint8_t gamefield[32][128];
int snake[16];

int food[2][5];
//int collide = {0, 0};
int score;


/* Initialize game logic */
void game_init(void) {    // ändra loopen

  insert_object(0, 0, 32, 128, game_background, gamefield, 0);

  // Initialize the snake
  int x = 40, y = 16, height = 1, width = 2, value = 3, length = 6;
  int i;
  for(i = 0; i <length; i++){
    insert_square(x, y, height, width, value, gamefield);
    insert_square(x-1, y+1, height, width, value, gamefield);
    x = x+3;
  }
  // Head towards right
  insert_square(x, y, height, width, value, gamefield);
  insert_square(x-1, y+1, height, 3, value, gamefield);

  snake[0] = 59;  // x+1
  snake[1] = 58;  // x
  snake[2] = 57;  // x-1
  snake[3] = 17;  // y+1
  snake[4] = 16;  // y
  snake[5] = 15;  // y-1
  snake[6] = 3;
  snake[7] = 41;
  snake[8] = 40;
  snake[9] = 39;
  snake[10] = 17;
  snake[11] = 16;
  snake[12] = 15;
  snake[13] = 3;
  snake[14] = 3;
  snake[15] = 3;
  // snake = [x+1, x, x-1, y+1, y, y-1, huvud-värdet, x+1, x, x-1, y+1, y, y-1, svans-värde, gamla huvudvärdet, gamla svansvärdet ]


  //  food[0] = {45, 22, 3, 3, 6};
  //  food[1] = {0, 0, 0, 0, 0};

  score = 0;

/*
    insert_object(1, 12 - (FOOD_REGULAR_HEIGHT / 2),
      FOOD_REGULAR_HEIGHT, FOOD_REGULAR_WIDTH, food_regular, gamefield, 0);
    insert_object(5, 12 - (FOOD_LIZARD_HEIGHT / 2),
      FOOD_LIZARD_HEIGHT, FOOD_LIZARD_WIDTH, food_lizard, gamefield, 0);
    insert_object(15, 12 - (FOOD_MOUSE_HEIGHT / 2),
      FOOD_MOUSE_HEIGHT, FOOD_MOUSE_WIDTH, food_mouse, gamefield, 0);
    insert_object(24, 12 - (FOOD_BIRD_HEIGHT / 2),
      FOOD_BIRD_HEIGHT, FOOD_BIRD_WIDTH, food_bird, gamefield, 0);
    insert_object(32, 12 - (FOOD_SPIDER_HEIGHT / 2),
      FOOD_SPIDER_HEIGHT, FOOD_SPIDER_WIDTH, food_spider, gamefield, 0);
    insert_object(42, 12 - (FOOD_FROG_HEIGHT / 2),
      FOOD_FROG_HEIGHT, FOOD_FROG_WIDTH, food_frog, gamefield, 0);
    insert_object(52, 12 - (FOOD_TURTLE_HEIGHT / 2),
      FOOD_TURTLE_HEIGHT, FOOD_TURTLE_WIDTH, food_turtle, gamefield, 0);
      */

}

/* Update program logic */
void game_update(int* buttons, int* switches) {
  // Up-button = 0
  // Right-button = 1
  // Left-button = 2
  // Down-button = 3
  // om turn = 0 --> ingen turn piece
  // om value = 0 --> ingen body piece
  // game_snake(int x, int y, int value, int head_value, int turn, int turn_value) 6 parameterar

  // insert_square(x, y, height, width, value, gamefield);
  if(buttons[0]){   // svänger uppåt
    snake[14] = snake[6];   // kopierar över head_value till gamla head_value
    insert_square(snake[0], snake[4], 2, 1, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 2, 1, 0, gamefield); // suddar ut
    insert_square(snake[2], snake[4], 2, 1, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 2, 1, 2, gamefield); // ändrar på föregånde huvud till en kroppsbit
    insert_square(snake[2], snake[3], 1, 1, 2, gamefield);
    insert_square(snake[0], snake[3], 1, 1, 2, gamefield);
    if(snake[14] == 3){    // om huvudet åkte mot höger innan
      snake[1] += 1;
      snake[4] -= 1;
    }
    if(snake[14] == 4){    // om huvudet åkte mot vänster innan
      snake[1] -= 2;
      snake[4] -= 1;                                                   // extrahuvud 1
    }
    snake[6] = 2;   // uppdaterar huvudvärdet
    snake[0] = snake[1]+1; // uppdaterar x+1 och x-1
    snake[2] = snake[1]-1;
    snake[3] = snake[4]+1;  // uppdaterar y+1 och y-1
    snake[5] = snake[4]-1;
  }

  if(buttons[1]){    // svänger höger
    snake[14] = snake[6];   // kopierar över head_value till gamla head_value
    insert_square(snake[1], snake[3], 1, 2, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 1, 2, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[5], 1, 2, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 1, 2, 3, gamefield); // ändrar föregåend huvud till en kroppsbit
    if(snake[14] == 2){    // om huvudet åkte uppåt innan
      insert_square(snake[1], snake[5], 1, 1, 3, gamefield); // ändrar föregåend huvud till en kroppsbit
      insert_square(snake[0], snake[3], 1, 1, 3, gamefield); // ändrar föregåend huvud till en kroppsbit
      snake[1] += 2;
      snake[4] -= 2;                                                   // extra huvud 2 som ger andra koordinater
    }
    if(snake[14] == 5){    // om huvudet åkte neråt innan
      insert_square(snake[0], snake[5], 1, 1, 3, gamefield); // ändrar föregåend huvud till en kroppsbit
      insert_square(snake[1], snake[3], 1, 1, 3, gamefield); // ändrar föregåend huvud till en kroppsbit
      snake[1] += 2;
      snake[4] += 1;
    }
    snake[6] = 3;
    snake[0] = snake[1]+1; // uppdaterar x+1 och x-1
    snake[2] = snake[1]-1;
    snake[3] = snake[4]+1;  // uppdaterar y+1 och y-1
    snake[5] = snake[4]-1;
  }

  if(buttons[2]){    // svänger vänster
    snake[14] = snake[6];   // kopierar över head_value till gamla head_value
    insert_square(snake[1], snake[5], 1, 2, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 1, 2, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[3], 1, 2, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 1, 2, 4, gamefield); // ändrar på föregånde huvud till en kroppsbit
    insert_square(snake[0], snake[3], 1, 1, 4, gamefield);
    insert_square(snake[0], snake[5], 1, 1, 4, gamefield);
    if(snake[14] == 2){    // om huvudet åkte uppåt innan
      snake[1] -= 1;
      snake[4] -= 2;         // extra huvud 3
    }
    if(snake[14] == 5){    // om huvudet åkte neråt innan
      snake[1] -= 1;
      snake[4] += 1;
    }
    snake[6] = 4;
    snake[0] = snake[1]+1; // uppdaterar x+1 och x-1
    snake[2] = snake[1]-1;
    snake[3] = snake[4]+1;  // uppdaterar y+1 och y-1
    snake[5] = snake[4]-1;
  }

  if(buttons[3]){    // svänger nedåt
    snake[14] = snake[6];   // kopierar över head_value till gamla head_value
    insert_square(snake[0], snake[4], 2, 1, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 2, 1, 0, gamefield); // suddar ut
    insert_square(snake[2], snake[4], 2, 1, 0, gamefield); // suddar ut
    insert_square(snake[1], snake[4], 2, 1, 5, gamefield);
    if(snake[14] == 3){    // om huvudet åkte mot höger innan
      insert_square(snake[2], snake[3], 1, 1, 5, gamefield); // ändrar på föregånde huvud till en kroppsbit
      insert_square(snake[0], snake[4], 1, 1, 5, gamefield);
      snake[1] += 1;
      snake[4] += 2;
    }
    if(snake[14] == 4){    // om huvudet åkte mot vänster innan
      insert_square(snake[2], snake[4], 1, 1, 5, gamefield);
      insert_square(snake[0], snake[3], 1, 1, 5, gamefield);
      snake[1] -= 2;
      snake[4] += 2;     // extrahuvud 4
    }
    snake[6] = 5;
    snake[0] = snake[1]+1; // uppdaterar x+1 och x-1
    snake[2] = snake[1]-1;
    snake[3] = snake[4]+1;  // uppdaterar y+1 och y-1
    snake[5] = snake[4]-1;
  }

}


// The snake moves forward
void game_move(void){

  int collision = 0;
  if(snake[6] == 2){   // uppåt ---------------------------------------------------------------------------
    if(snake[6] == snake[14]){
      insert_square(snake[0], snake[5], 1, 1, 0, gamefield); // ändrar föregående från huvud till kropp
      snake[4] -= 3; // uppdaterar y
      snake[3] = snake[4]+1;  // uppdaterar y+1 och y-1
      snake[5] = snake[4]-1;
      snake[4] = snake[4] < 1? snake[4]+23 : snake[4];    // kollar så att vi är på spelplanen
      snake[3] = snake[3] < 1? snake[3]+23 : snake[3];
      snake[5] = snake[5] < 1? snake[5]+23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[0], snake[3], 1, 1, 2, gamefield);
      insert_square(snake[1], snake[4], 1, 2, 2, gamefield);
      insert_square(snake[1], snake[5], 1, 2, 2, gamefield);
    }
    if(snake[14] == 3){  // om höger innan      ************          *********          *************
      snake[1] = snake[1] > 126? snake[1]-126 : snake[1]; // kollar så att vi är på spelplanen
      snake[0] = snake[0] > 126? snake[0]-126 : snake[0];
      snake[2] = snake[2] > 126? snake[2]-126 : snake[2];
      snake[4] = snake[4] < 1? snake[4]+23 : snake[4];    // kollar så att vi är på spelplanen
      snake[3] = snake[3] < 1? snake[3]+23 : snake[3];
      snake[5] = snake[5] < 1? snake[5]+23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[0], snake[3], 1, 1, 2, gamefield);
      insert_square(snake[1], snake[4], 1, 2, 2, gamefield);
      insert_square(snake[1], snake[5], 1, 2, 2, gamefield);
      snake[14] = 2; // uppdaterar gamla huvudvärdet
    }

    if(snake[14] == 4){  // om vänster innan  ************          *********          *************
      snake[1] = snake[1] < 1? snake[1]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
      snake[0] = snake[0] < 1? snake[0]+126 : snake[0];
      snake[2] = snake[2] < 1? snake[2]+126 : snake[2];
      snake[4] = snake[4] < 1? snake[4]+23 : snake[4];    // kollar så att vi är på spelplanen i Y-LED
      snake[3] = snake[3] < 1? snake[3]+23 : snake[3];
      snake[5] = snake[5] < 1? snake[5]+23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[1], snake[3], 1, 1, 2, gamefield);
      insert_square(snake[1], snake[4], 1, 2, 2, gamefield);
      insert_square(snake[1], snake[5], 1, 2, 2, gamefield);
      snake[14] = 2; // uppdaterar gamla huvudvärdet
    }
  }

  if(snake[6] == 3){ // åt höger  ---------------------------------------------------------------------
    if(snake[6] == snake[14]){
      insert_square(snake[0], snake[3], 1, 1, 0, gamefield); // ändrar föregående från huvud till kropp
      snake[1] += 3;    //x
      snake[0] = snake[1]+1; // uppdaterar x+1 och x-1
      snake[2] = snake[1]-1;
      snake[1] = snake[1] > 126? snake[1]-126 : snake[1]; // kollar så att vi är på spelplanen
      snake[0] = snake[0] > 126? snake[0]-126 : snake[0];
      snake[2] = snake[2] > 126? snake[2]-126 : snake[2];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);

      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[2], snake[3], 1, 1, 3, gamefield);
      insert_square(snake[1], snake[4], 2, 1, 3, gamefield);
      insert_square(snake[0], snake[4], 2, 1, 3, gamefield);

      // kolla så att x och y är på spelplanen
      // printa ut huvudet
      // kolla om svans ska tas bort (ska bara tas bort om ormen inte krockar med något)
      // Om mat - ta ej bort svans och skriv ut huvud
      // om orm eller vägg - förlorar
      // om inget - ta bort svans och skriv ut huvud
    }

    if(snake[14] == 2){   // om uppåt innan      ************          *********          *************
      snake[1] = snake[1] > 126? snake[1]-126 : snake[1]; // kollar så att vi är på spelplanen
      snake[0] = snake[0] > 126? snake[0]-126 : snake[0];
      snake[2] = snake[2] > 126? snake[2]-126 : snake[2];
      snake[4] = snake[4] < 1? snake[4]+23 : snake[4];    // kollar så att vi är på spelplanen
      snake[3] = snake[3] < 1? snake[3]+23 : snake[3];
      snake[5] = snake[5] < 1? snake[5]+23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      insert_square(snake[2], snake[4], 1, 1, 3, gamefield);   // skriver ut extrahuvud
      insert_square(snake[1], snake[4], 2, 1, 3, gamefield);
      insert_square(snake[0], snake[4], 2, 1, 3, gamefield);
      snake[14] = 3; // uppdaterar gamla huvudvärdet

      // kolla så att x och y är på spelplanen
      // skriver ut huvud (eventuellt extrahuvud)
      // uppdatera det gamla huvudvärdet
      // kolla kollision
      // kolla om svans ska tas bort (ska bara tas bort om ormen inte krockar med något)
    }

    if(snake[14] == 5){   // om nedåt innan       ************          *********          *************
      snake[1] = snake[1] > 126? snake[1]-126 : snake[1]; // kollar så att vi är på spelplanen
      snake[0] = snake[0] > 126? snake[0]-126 : snake[0];
      snake[2] = snake[2] > 126? snake[2]-126 : snake[2];
      snake[4] = snake[4] > 23? snake[4]-23 : snake[4]; // kollar så att vi är på spelplanen
      snake[3] = snake[3] > 23? snake[3]-23 : snake[3];
      snake[5] = snake[5] > 23? snake[5]-23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      insert_square(snake[1], snake[4], 2, 1, 3, gamefield);   // skriver ut huvud
      insert_square(snake[0], snake[4], 2, 1, 3, gamefield);
      insert_square(snake[2], snake[3], 1, 1, 3, gamefield);
      snake[14] = 3;  // uppdatera det gamla huvudvärdet
    }
  }

  if(snake[6] == 4){  // åt vänster -----------------------------------------------------------------------
    if(snake[6] == snake[14]){
      insert_square(snake[2], snake[3], 1, 1, 0, gamefield); // ändrar föregående från huvud till kropp
      snake[1] -= 3;    //x
      snake[0] = snake[1]+1; // uppdaterar x+1 och x-1
      snake[2] = snake[1]-1;
      snake[1] = snake[1] < 1? snake[1]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
      snake[0] = snake[0] < 1? snake[0]+126 : snake[0];
      snake[2] = snake[2] < 1? snake[2]+126 : snake[2];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);

      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[0], snake[3], 1, 1, 4, gamefield);
      insert_square(snake[1], snake[4], 2, 1, 4, gamefield);
      insert_square(snake[2], snake[4], 2, 1, 4, gamefield);

    }
    if(snake[14] == 2){  // om åkte uppåt innan    ************          *********          *************
      snake[1] = snake[1] < 1? snake[1]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
      snake[0] = snake[0] < 1? snake[0]+126 : snake[0];
      snake[2] = snake[2] < 1? snake[2]+126 : snake[2];
      snake[4] = snake[4] < 1? snake[4]+23 : snake[4];    // kollar så att vi är på spelplanen
      snake[3] = snake[3] < 1? snake[3]+23 : snake[3];
      snake[5] = snake[5] < 1? snake[5]+23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      insert_square(snake[0], snake[4], 1, 1, 4, gamefield);   // skriver ut extrahuvud
      insert_square(snake[1], snake[4], 2, 1, 4, gamefield);
      insert_square(snake[2], snake[4], 2, 1, 4, gamefield);
      snake[14] = 4; // uppdaterar gamla huvudvärdet

    }
    if(snake[14] == 5){ // om åkte nedåt innan     ************          *********          *************
      snake[1] = snake[1] < 1? snake[1]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
      snake[0] = snake[0] < 1? snake[0]+126 : snake[0];
      snake[2] = snake[2] < 1? snake[2]+126 : snake[2];
      snake[4] = snake[4] > 23? snake[4]-23 : snake[4]; // kollar så att vi är på spelplanen
      snake[3] = snake[3] > 23? snake[3]-23 : snake[3];
      snake[5] = snake[5] > 23? snake[5]-23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      insert_square(snake[0], snake[3], 1, 1, 4, gamefield);   // skriver ut huvud
      insert_square(snake[1], snake[4], 2, 1, 4, gamefield);
      insert_square(snake[2], snake[4], 2, 1, 4, gamefield);
      snake[14] = 4; // uppdaterar gamla huvudvärdet
    }
  }


  if(snake[6] == 5){  // nedåt ----------------------------------------------------------------------------
    if(snake[6] == snake[14]){
      insert_square(snake[0], snake[3], 1, 1, 0, gamefield); // ändrar föregående från huvud till kropp
      snake[4] += 3; // uppdaterar y
      snake[3] = snake[4]+1;  // uppdaterar y+1 och y-1
      snake[5] = snake[4]-1;
      snake[4] = snake[4] > 23? snake[4]-23 : snake[4]; // kollar så att vi är på spelplanen
      snake[3] = snake[3] > 23? snake[3]-23 : snake[3];
      snake[5] = snake[5] > 23? snake[5]-23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[0], snake[5], 1, 1, 5, gamefield);
      insert_square(snake[1], snake[4], 1, 2, 5, gamefield);
      insert_square(snake[1], snake[3], 1, 2, 5, gamefield);
    }
    if(snake[14] == 3){   // om åkte emot höger innan
      snake[1] = snake[1] > 126? snake[1]-126 : snake[1]; // kollar så att vi är på spelplanen
      snake[0] = snake[0] > 126? snake[0]-126 : snake[0];
      snake[2] = snake[2] > 126? snake[2]-126 : snake[2];
      snake[4] = snake[4] > 23? snake[4]-23 : snake[4]; // kollar så att vi är på spelplanen
      snake[3] = snake[3] > 23? snake[3]-23 : snake[3];
      snake[5] = snake[5] > 23? snake[5]-23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[0], snake[5], 1, 1, 5, gamefield);
      insert_square(snake[1], snake[4], 1, 2, 5, gamefield);
      insert_square(snake[1], snake[3], 1, 2, 5, gamefield);
      snake[14] = 5; // uppdaterar gamla huvudvärdet
    }
    if(snake[14] == 4){   // om åkte mot vänster innan
      snake[1] = snake[1] < 1? snake[1]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
      snake[0] = snake[0] < 1? snake[0]+126 : snake[0];
      snake[2] = snake[2] < 1? snake[2]+126 : snake[2];
      snake[4] = snake[4] > 23? snake[4]-23 : snake[4]; // kollar så att vi är på spelplanen
      snake[3] = snake[3] > 23? snake[3]-23 : snake[3];
      snake[5] = snake[5] > 23? snake[5]-23 : snake[5];

      // Kollar om vi krockar med något och anger om vi ska ta bort svansen eller inte
      //collision = game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]);
      //if(collision == 1){         // avsluta spelet
      //  game_end_init(score);
      //  gamestate = GAME_END;
      //  return;
      //}
      // skriver ut huvudet
      insert_square(snake[1], snake[5], 1, 1, 5, gamefield);
      insert_square(snake[1], snake[4], 1, 2, 5, gamefield);
      insert_square(snake[1], snake[3], 1, 2, 5, gamefield);
      snake[14] = 5; // uppdaterar gamla huvudvärdet
    }

  }

  if (collision == 0){  // ta bort svansen ------------------------------------------------------------------
    if(snake[13] == 2){
      if(snake[15] == 2){
        insert_square(snake[8], snake[10], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[11], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[12], 1, 2, 0, gamefield);

        snake[11] -= 3;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[11] = snake[11] < 1? snake[11]+23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] < 1? snake[10]+23 : snake[10];
        snake[12] = snake[12] < 1? snake[12]+23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 2;
      }
      if(snake[15] == 3){
        insert_square(snake[7], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[8], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[9], snake[11], 2, 1, 0, gamefield);

        snake[8] += 1;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] += 1;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] > 126? snake[8]-126 : snake[8]; // kollar så att vi är på spelplanen
        snake[7] = snake[7] > 126? snake[7]-126 : snake[7];
        snake[9] = snake[9] > 126? snake[9]-126 : snake[9];
        snake[11] = snake[11] > 23? snake[11]-23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] > 23? snake[10]-23 : snake[10];
        snake[12] = snake[12] > 23? snake[12]-23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 2;
      }
      if(snake[15] == 4){
        insert_square(snake[7], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[8], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[9], snake[11], 2, 1, 0, gamefield);

        snake[8] -= 2;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] -= 1;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] < 1? snake[8]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
        snake[7] = snake[7] < 1? snake[7]+126 : snake[0];
        snake[9] = snake[9] < 1? snake[9]+126 : snake[2];
        snake[11] = snake[11] < 1? snake[11]+23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] < 1? snake[10]+23 : snake[10];
        snake[12] = snake[12] < 1? snake[12]+23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 2;
      }
    }

    if(snake[13] == 3){
      if(snake[15] == 3){
        insert_square(snake[7], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[8], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[9], snake[11], 2, 1, 0, gamefield);

        snake[8] += 3;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;

        snake[8] = snake[8] > 126? snake[8]-126 : snake[8]; // kollar så att vi är på spelplanen
        snake[7] = snake[7] > 126? snake[7]-126 : snake[7];
        snake[9] = snake[9] > 126? snake[9]-126 : snake[9];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 3;
      }
      if(snake[15] == 5){
        insert_square(snake[8], snake[10], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[11], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[12], 1, 2, 0, gamefield);

        snake[8] += 2;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] += 1;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] > 126? snake[8]-126 : snake[8]; // kollar så att vi är på spelplanen
        snake[7] = snake[7] > 126? snake[7]-126 : snake[7];
        snake[9] = snake[9] > 126? snake[9]-126 : snake[9];
        snake[11] = snake[11] > 23? snake[11]-23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] > 23? snake[10]-23 : snake[10];
        snake[12] = snake[12] > 23? snake[12]-23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 3;
      }
      if(snake[15] == 2){
        insert_square(snake[8], snake[10], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[11], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[12], 1, 2, 0, gamefield);

        snake[8] += 2;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] -= 2;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] > 126? snake[8]-126 : snake[8]; // kollar så att vi är på spelplanen
        snake[7] = snake[7] > 126? snake[7]-126 : snake[7];
        snake[9] = snake[9] > 126? snake[9]-126 : snake[9];
        snake[11] = snake[11] < 1? snake[11]+23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] < 1? snake[10]+23 : snake[10];
        snake[12] = snake[12] < 1? snake[12]+23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 3;
      }
    }

    if(snake[13] == 4){
      if(snake[15] == 4){
        insert_square(snake[7], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[8], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[9], snake[11], 2, 1, 0, gamefield);

        snake[8] -= 3;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;

        snake[8] = snake[8] < 1? snake[8]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
        snake[7] = snake[7] < 1? snake[7]+126 : snake[0];
        snake[9] = snake[9] < 1? snake[9]+126 : snake[2];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 4;
      }
      if(snake[15] == 5){
        insert_square(snake[8], snake[10], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[11], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[12], 1, 2, 0, gamefield);

        snake[8] -= 1;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] += 1;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] < 1? snake[8]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
        snake[7] = snake[7] < 1? snake[7]+126 : snake[0];
        snake[9] = snake[9] < 1? snake[9]+126 : snake[2];
        snake[11] = snake[11] > 23? snake[11]-23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] > 23? snake[10]-23 : snake[10];
        snake[12] = snake[12] > 23? snake[12]-23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 4;
      }
      if(snake[15] == 2){
        insert_square(snake[8], snake[10], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[11], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[12], 1, 2, 0, gamefield);

        snake[8] -= 1;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] -= 2;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] < 1? snake[8]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
        snake[7] = snake[7] < 1? snake[7]+126 : snake[0];
        snake[9] = snake[9] < 1? snake[9]+126 : snake[2];
        snake[11] = snake[11] < 1? snake[11]+23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] < 1? snake[10]+23 : snake[10];
        snake[12] = snake[12] < 1? snake[12]+23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 4;
      }
    }
    if(snake[13] == 5){
      if(snake[15] == 5){
        insert_square(snake[8], snake[10], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[11], 1, 2, 0, gamefield);
        insert_square(snake[8], snake[12], 1, 2, 0, gamefield);

        snake[11] += 3;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[11] = snake[11] > 23? snake[11]-23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] > 23? snake[10]-23 : snake[10];
        snake[12] = snake[12] > 23? snake[12]-23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 5;
      }
      if(snake[15] == 3){
        insert_square(snake[7], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[8], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[9], snake[11], 2, 1, 0, gamefield);

        snake[8] += 1;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] += 2;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] > 126? snake[8]-126 : snake[8]; // kollar så att vi är på spelplanen
        snake[7] = snake[7] > 126? snake[7]-126 : snake[7];
        snake[9] = snake[9] > 126? snake[9]-126 : snake[9];
        snake[11] = snake[11] > 23? snake[11]-23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] > 23? snake[10]-23 : snake[10];
        snake[12] = snake[12] > 23? snake[12]-23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 5;
      }
      if(snake[15] == 4){
        insert_square(snake[7], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[8], snake[11], 2, 1, 0, gamefield);
        insert_square(snake[9], snake[11], 2, 1, 0, gamefield);

        snake[8] -= 2;
        snake[7] = snake[8]+1;
        snake[9] = snake[8]-1;
        snake[11] += 2;
        snake[10] = snake[11]+1;
        snake[12] = snake[11]-1;

        snake[8] = snake[8] < 1? snake[8]+126 : snake[1]; // kollar så att vi är på spelplanen i X-LED
        snake[7] = snake[7] < 1? snake[7]+126 : snake[0];
        snake[9] = snake[9] < 1? snake[9]+126 : snake[2];
        snake[11] = snake[11] > 23? snake[11]-23 : snake[11]; // kollar så att vi är på spelplanen
        snake[10] = snake[10] > 23? snake[10]-23 : snake[10];
        snake[12] = snake[12] > 23? snake[12]-23 : snake[12];

        snake[13] = gamefield[snake[11]][snake[8]];
        snake[15] = 5;
      }
    }
  }

}
/*
int game_collisiondetect1(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]){ // när ormen åker upp/ner
  int x;
  int y;
  int collision = 0;
  // kolla om någon av bitarna kolliderar med något (är det större än noll?)
  // om ej så gör inget, annars:
  // spara det x-och y-värde som ger kollision i två lokala variabler
  // kör en ifsats på dem variblerna och kolla
  // om vägg eller orm: avsluta spelet
  // om det är >5 så skicka till game_collision()
  return collision;
}

int game_collisiondetect2(snake[0], snake[1], snake[2], snake[3], snake[4], snake[5]){ // när ormen åker mot vänster/höger
  int x;
  int y;
  int collision = 0;
  // kolla om någon av bitarna kolliderar med något
  // om ej så gör inget, annars:
  // spara det x-och y-värde som ger kollision i två lokala variabler
  // kör en ifsats på dem variblerna och kolla
  // om vägg eller orm: avsluta spelet
  // om det är >5 så skicka till game_collision() och ändra collision till rätt tal
  return collision;
}

void game_collision(collideX, collideY){
  int i;
  for(i = 0; i < 2; i++){
    if(collideX >= food[i][0] && collideX < (food[i][0]+food[i][2])
       && collideY >= food[i][1] && collideY < (food[i][1]+food[i][3])){
      insert_square(food[i][0], food[i][1], food[i][3], food[i][2], 0, gamefield);
      score += (food[i][5]*speed)
    }
  }

}
*/
/* Draw game */
void game_draw(void) {
    display_full_bin(gamefield);
}
