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
int snake[8];

int food[2][5];
//int collide = {0, 0};
int score;

void draw_head(int x, int y, int dir, int flip) {

  int xr = x + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : x + 1;
  int yd = y + 1 >= GAMEFIELD_LEFT + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : y + 1;

  if(dir == 2 || dir == 5) {
    int yu = y - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : y - 1;

    // Fill head
    insert_square(x, yu, 1, 2, dir, gamefield);
    insert_square(x, y, 1, 2, dir, gamefield);
    insert_square(x, yd, 1, 2, dir, gamefield);

    // Remove odd bit
    if(dir == 2) { // Up
      if(!flip)
        insert_square(x, yd, 1, 1, 0, gamefield);
      else
        insert_square(xr, yd, 1, 1, 0, gamefield);
    }
    if(dir == 5) { // Down
      if(!flip)
        insert_square(x, yu, 1, 1, 0, gamefield);
      else
        insert_square(xr, yu, 1, 1, 0, gamefield);
    }
  }

  if(dir == 3 || dir == 4) {
    int xl = x - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : x - 1;

    // Fill head
    insert_square(xl, y, 2, 1, dir, gamefield);
    insert_square(x, y, 2, 1, dir, gamefield);
    insert_square(xr, y, 2, 1, dir, gamefield);

    // Remove odd bit
    if(dir == 3) { // Right
      if(!flip)
        insert_square(xl, y, 1, 1, 0, gamefield);
      else
        insert_square(xl, yd, 1, 1, 0, gamefield);
    }
    if(dir == 4) { // Down
      if(!flip)
        insert_square(xr, y, 1, 1, 0, gamefield);
      else
        insert_square(xr, yd, 1, 1, 0, gamefield);
    }
  }
}

/* Initialize game logic */
void game_init(void) {    // ändra loopen

  insert_object(0, 0, 32, 128, game_background, gamefield, 0);

  // Initialize the snake
  snake[0] = 63;  // head x
  snake[1] = 14;  // head y
  snake[2] = 3;   // head direction
  snake[3] = 63 - 6*3;  // tail x
  snake[4] = 14;  // tail y
  snake[5] = 3;   // tail direction
  snake[6] = 3;   // head previous direction
  snake[7] = 3;   // tail previous direction

  draw_head(snake[0], snake[1], snake[2], 0);

  int i;
  for(i = 1; i < 7; i++){
    insert_square(snake[0] - i*3, snake[1], 1, 2, snake[2], gamefield);
    insert_square(snake[0] - i*3 - 1, snake[1] + 1, 1, 2, snake[2], gamefield);
  }

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

  if(prevgamestate == GAME) {
    if(buttons[0]){
      if(snake[2] != 5)     // Don't turn in opposite direction
        snake[2] = 2;       // Turn up
    }
    if(buttons[1]){
      if(snake[2] != 4)     // Don't turn in opposite direction
        snake[2] = 3;       // Turn right
    }
    if(buttons[2]){
      if(snake[2] != 3)     // Don't turn in opposite direction
        snake[2] = 4;       // Turn left
    }
    if(buttons[3]){
      if(snake[2] != 2)     // Don't turn in opposite direction
        snake[2] = 5;       // Turn down
    }
  }
}


// The snake moves forward
void game_move(void){
  int fliphead = 0, collision = 0;
  int ty = snake[1] - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : snake[1] - 1;

  // Update direction bit
  insert_square(snake[0], snake[1], 1, 1, snake[2], gamefield);

  // Update position and old head
  switch(snake[2]) {
    case 2: // Up
      if(snake[6] != snake[2]) {
        if(snake[6] == 3) { // Previously right
          // Remove head bit
          insert_square(snake[0] + 1, snake[1], 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] + 1;
          snake[1] = snake[1] - 1;
        }
        else if(snake[6] == 4) { // Previously left
          // Remove head bit
          insert_square(snake[0] - 1, snake[1], 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] - 2 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : snake[0] - 2;
          snake[1] = snake[1] - 1;
          fliphead = 1;
        }
      }
      else {
        // Remove head bit
        insert_square(snake[0] + 1, ty, 1, 1, 0, gamefield);

        // Update position
        snake[1] = snake[1] - 3 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 2 : snake[1] - 3;
      }
      break;
    case 3: // Right
      if(snake[6] != snake[2]) {
        if(snake[6] == 2) { // Previously up
          // Remove head bit
          insert_square(snake[0] + 1, ty, 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] + 2 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT + 1 : snake[0] + 2;
          snake[1] = snake[1] - 2 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 2 : snake[1] - 2;
          fliphead = 1;
        }
        else if(snake[6] == 5) { // Previously down
          // Remove head bit
          insert_square(snake[0] + 1, snake[1] + 1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] + 2 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT + 1 : snake[0] + 2;
          snake[1] = snake[1] + 1;
        }
      }
      else {
        // Remove head bit
        insert_square(snake[0] + 1, snake[1] + 1, 1, 1, 0, gamefield);

        // Update position
        snake[0] = snake[0] + 3 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT + 1: snake[0] + 3;
      }
      break;
    case 4: // Left
      if(snake[6] != snake[2]) {
        if(snake[6] == 2) { // Previously up
          // Remove head bit
          insert_square(snake[0], ty, 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] - 1;
          snake[1] = snake[1] - 2 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 2 : snake[1] - 2;
          fliphead = 1;
        }
        else if(snake[6] == 5) {  // Previously down
          // Remove head bit
          insert_square(snake[0], snake[1] + 1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] - 1;
          snake[1] = snake[1] + 1;
        }
      }
      else {
        // Remove head bit
        insert_square(snake[0] - 1, snake[1] + 1, 1, 1, 0, gamefield);

        // Update position
        snake[0] = snake[0] - 3 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 2: snake[0] - 3;
      }
      break;
    case 5: // Down
      if(snake[6] != snake[2]) {
        if(snake[6] == 3) { // Previously right
          // Remove head bit
          insert_square(snake[0] + 1, snake[1] + 1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] + 1;
          snake[1] = snake[1] + 2 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : snake[1] + 2;
        }
        if(snake[6] == 4) { // Previously left
          // Remove head bit
          insert_square(snake[0] - 1, snake[1] + 1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = snake[0] - 2 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : snake[0] - 2;
          snake[1] = snake[1] + 2 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : snake[1] + 2;
          fliphead = 1;
        }
      }
      else {
        // Remove head bit
        insert_square(snake[0] + 1, snake[1] + 1, 1, 1, 0, gamefield);

        // Update position
        snake[1] = snake[1] + 3 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP + 1 : snake[1] + 3;
      }
      break;
  }

  if(!collision) {
    int next = gamefield[snake[4]][snake[3]];
    int yu = snake[4] - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : snake[4] - 1;
    if(snake[5] == 2 || snake[5] == 5) {
      insert_square(snake[3], yu, 1, 2, 0, gamefield);
      insert_square(snake[3], snake[4], 2, 2, 0, gamefield);
    }
    else if(snake[5] == 3 || snake[5] == 4) {
      insert_square(snake[3] - 1, snake[4], 2, 3, 0, gamefield);
    }
  }

  snake[6] = snake[2];  // Copy direction to previous direction
  draw_head(snake[0], snake[1], snake[2], fliphead);

  // Draw score
  insert_num(23, 26, score, gamefield, 0);

/*
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
  }*/
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
