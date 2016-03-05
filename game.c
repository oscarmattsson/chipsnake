/* game.c

   This file written 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include <stdlib.h>

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

int game_generate_position(int* x, int* y){

  *x = rand() % 120; // random int in range 0 & 119
  *y = rand() % 20; // random int in range 0 & 19
  // x,y must be be larger than zero if walls are activated
  if(*x == 0){
    *x += 1;
  }
  if(*y == 0){
    *y += 1;
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

  // Initialize random seed:
  srand((unsigned)TMR2);

  // Generate food
  food[0][2] = FOOD_REGULAR_WIDTH;
  food[0][3] = FOOD_REGULAR_HEIGHT;
  food[0][4] = FOOD_REGULAR_VALUE;
  game_generate_food_regular();

  // No special food from the beginning
  for(i = 0; i < 5; i++){
    food[1][i] = 0;
  }

  // Initialize score
  score = 0;

}

void game_generate_food_regular(void){
  do{
    int collision = 0;
    game_generate_position(&food[0][0], &food[0][1])
    int i, j;
    for(j = food[0][1]; j < (food[0][1] + food[0][3]); j++){
      for(i = food[0][0]; i < (food[0][0] + food[0][2]); i++){
        if(gamefield[j][i] != 0){
          collision = 1;
        }
      }
    }
  } while(collision == 1);

  // print ut special food
  insert_object(food[0][0], food[0][1], food[0][3], food[0][2], food_regular, gamefield, 0);
}

void game_generate_food_special(void){
  uint8_t* pattern;

  type = rand() % 6;
  if(type == 0){
    food[1][2] = FOOD_LIZARD_WIDTH;
    food[1][3] = FOOD_LIZARD_HEIGHT;
    food[1][4] = FOOD_LIZARD_VALUE;
    pattern = food_lizard;
  }
  if(type == 1){
    food[1][2] = FOOD_MOUSE_WIDTH;
    food[1][3] = FOOD_MOUSE_HEIGHT;
    food[1][4] = FOOD_MOUSE_VALUE;
    pattern = food_mouse;
  }
  if(type == 2){
    food[1][2] = FOOD_BIRD_WIDTH;
    food[1][3] = FOOD_BIRD_HEIGHT;
    food[1][4] = FOOD_BIRD_VALUE;
    pattern = food_bird;
  }
  if(type == 3){
    food[1][2] = FOOD_SPIDER_WIDTH;
    food[1][3] = FOOD_SPIDER_HEIGHT;
    food[1][4] = FOOD_SPIDER_VALUE;
    pattern = food_spider
  }
  if(type == 4){
    food[1][2] = FOOD_FROG_WIDTH;
    food[1][3] = FOOD_FROG_HEIGHT;
    food[1][4] = FOOD_FROG_VALUE;
    pattern = food_frog;
  }
  if(type == 5){
    food[1][2] = FOOD_TURTLE_WIDTH;
    food[1][3] = FOOD_TURTLE_HEIGHT;
    food[1][4] = FOOD_TURTLE_VALUE;
    pattern = food_turtle;
  }

  do{
    int collision = 0;
    game_generate_position(&food[1][0], &food[1][1])
    int i, j;
    for(j = food[1][1]; j < (food[1][1] + food[1][3]); j++){
      for(i = food[1][0]; i < (food[1][0] + food[1][2]); i++){
        if(gamefield[j][i] != 0){
          collision = 1;
        }
      }
    }
  } while(collision == 1);

  // print ut special food
  insert_object(food[1][0], food[1][1], food[1][3], food[1][2], pattern, gamefield, 0);
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
  collision = game_collisiondetect();
  if(!collision) {    // removes the tail
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
}



int game_collisiondetect(void){
  int x;
  int y;
  int collision = 0;
  if(snake[2] == 2 || snake[2] == 5){
    if(gamefield[snake[1]-1][snake[0]] != 0){
      x = snake[0];
      y = snake[1] - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : snake[1] - 1;
    }
    if(gamefield[snake[1]-1][snake[0]+1] != 0){
      x = snake[0] + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : snake[0] + 1;
      y = snake[1] - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : snake[1] - 1;
    }
  }
  if(snake[2] == 3 || snake[2] == 4){
    if(gamefield[snake[1]][snake[0]-1] != 0){
      x = snake[0]-1;
      y = snake[1];
    }
    if(gamefield[snake[1]+1][snake[0]-1] != 0){
      x = snake[0]-1;
      y = snake[1]+1;
    }
  }
  // dessa är gemensamma för båda fallen
  if(gamefield[snake[1]][snake[0]] != 0){
    x = snake[0];
    y = snake[1];
  }
  if(gamefield[snake[1]][snake[0]+1] != 0){
    x = snake[0] + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : snake[0] + 1;
    y = snake[1];
  }
  if(gamefield[snake[1]+1][snake[0]] != 0){
    x = snake[0];
    y = snake[1]+1;
  }
  if(gamefield[snake[1]+1][snake[0]+1] != 0){
    x = snake[0] + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : snake[0] + 1;
    y = snake[1]+1;
  }

  if(gamefield[y][x] >= 1 && gamefield[y][x] < 6){
    game_end_init(score);
    gamestate = GAME_END;
    return;
  }
  if(gamefield[y][x] == 6){
    collision = 1;
    game_collision(x, y);
  }

  return collision;
}


void game_collision(collideX, collideY){
  int i;
  for(i = 0; i < 2; i++){
    if(collideX >= food[i][0] && collideX < (food[i][0]+food[i][2])
       && collideY >= food[i][1] && collideY < (food[i][1]+food[i][3])){
      insert_square(food[i][0], food[i][1], food[i][3], food[i][2], 0, gamefield);
      score += (food[i][5]*speed)
      if(i == 0){
        game_generate_food_regular();
      }
    }
  }

}

/* Draw game */
void game_draw(void) {
    display_full_bin(gamefield);
}
