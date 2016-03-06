/* game.c

   This file written 2016 by Moa Thoren
   Modified 2016 by Oscar Mattsson

   For copyright and licensing, see file COPYING

   */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "chipsnake.h"  /* Declatations for game */
#include "game-defs.h"

uint8_t gamefield[32][128];
uint8_t snake[8];

uint8_t food[2][5];
uint8_t counter;
int score;

void draw_head(uint8_t x, uint8_t y, uint8_t dir, uint8_t flip) {

  uint8_t xr = x + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : x + 1;
  uint8_t yd = y + 1 >= GAMEFIELD_LEFT + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : y + 1;

  if(dir == 2 || dir == 5) {
    uint8_t yu = y - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : y - 1;

    // Fill head
    insert_square(x, yu, 1, 1, dir, gamefield);
    insert_square(xr, yu, 1, 1, dir, gamefield);
    insert_square(x, y, 1, 1, dir, gamefield);
    insert_square(xr, y, 1, 1, dir, gamefield);
    insert_square(x, yd, 1, 1, dir, gamefield);
    insert_square(xr, yd, 1, 1, dir, gamefield);

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
    uint8_t xl = x - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : x - 1;

    // Fill head
    insert_square(xl, y, 1, 1, dir, gamefield);
    insert_square(xl, yd, 1, 1, dir, gamefield);
    insert_square(x, y, 1, 1, dir, gamefield);
    insert_square(x, yd, 1, 1, dir, gamefield);
    insert_square(xr, y, 1, 1, dir, gamefield);
    insert_square(xr, yd, 1, 1, dir, gamefield);

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

void game_generate_position(uint8_t* x, uint8_t* y){

  *x = (seed * (counter + snake[3]) * 123) % 120; // random int in range 0 & 119
  *y = (seed * (counter + snake[0]) * 21) % 20; // random int in range 0 & 19
  // x,y must be be larger than zero if walls are activated
  if(*x == 0){
    (*x) += 1;
  }
  if(*y == 0){
    (*y) += 1;
  }
}

void game_generate_food_regular(void){
  uint8_t collision;
  do{
    collision = 0;
    game_generate_position(&food[0][0], &food[0][1]);
    uint8_t i, j;
    for(j = food[0][1]; j < (food[0][1] + food[0][3]); j++){
      for(i = food[0][0]; i < (food[0][0] + food[0][2]); i++){
        if(gamefield[j][i] != 0){
          collision = 1;
          seed += 1;
        }
      }
    }
  } while(collision == 1);

  // print ut special food
  insert_object(food[0][0], food[0][1], food[0][3], food[0][2], food_regular, gamefield, 0);
}

void game_generate_food_special(void){
  uint8_t const* pattern;

  int type = (seed * (counter + snake[4])) % 6;
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
    pattern = food_spider;
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
  uint8_t collision;
  do{
    collision = 0;
    game_generate_position(&food[1][0], &food[1][1]);
    uint8_t i, j;
    for(j = food[1][1]; j < (food[1][1] + food[1][3]); j++){
      for(i = food[1][0]; i < (food[1][0] + food[1][2]); i++){
        if(gamefield[j][i] != 0){
          collision = 1;
          seed += 1;
        }
      }
    }
  } while(collision == 1);

  // print ut special food
  insert_object(food[1][0], food[1][1], food[1][3], food[1][2], pattern, gamefield, 0);
}

/* Initialize game logic */
void game_init(void) {

  // Initialize screen
  insert_square(0, 0, 25, 128, 0, gamefield);
  insert_square(0, 25, 7, 128, 1, gamefield);
  insert_object(0, 25, 7, 23, pattern_score, gamefield, 0);
  insert_string(98, 26, "MENU", gamefield, 0);
  insert_object(122, 26, 5, 5, switch_up, gamefield, 0);

  // Initialize the snake
  snake[0] = 63;  // head x
  snake[1] = 14;  // head y
  snake[2] = 3;   // head direction
  snake[3] = 63 - 6*3;  // tail x
  snake[4] = 14;  // tail y
  snake[5] = 3;   // tail direction
  snake[6] = 3;   // head previous direction
  snake[7] = 3;   // tail previous direction

  // Draw head
  draw_head(snake[0], snake[1], snake[2], 0);

  // Draw body
  uint8_t i;
  for(i = 1; i < 7; i++){
    insert_square(snake[0] - i*3, snake[1], 1, 2, snake[2], gamefield);
    insert_square(snake[0] - i*3 - 1, snake[1] + 1, 1, 2, snake[2], gamefield);
  }

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

  // Draw score
  insert_num(23, 26, score, gamefield, 0);
}

/* Update program logic */
void game_update(int* buttons, int* switches) {
  if(prevgamestate == GAME) {
    if(buttons[0]){
      if(snake[6] != 5)     // Don't turn in opposite direction
        snake[2] = 2;       // Turn up
    }
    if(buttons[1]){
      if(snake[6] != 4)     // Don't turn in opposite direction
        snake[2] = 3;       // Turn right
    }
    if(buttons[2]){
      if(snake[6] != 3)     // Don't turn in opposite direction
        snake[2] = 4;       // Turn left
    }
    if(buttons[3]){
      if(snake[6] != 2)     // Don't turn in opposite direction
        snake[2] = 5;       // Turn down
    }
  }
}


void game_collision(uint8_t x, uint8_t y){
  uint8_t i;
  for(i = 0; i < 2; i++){
    if(x >= food[i][0] &&
       x < (food[i][0]+food[i][2]) &&
       y >= food[i][1] &&
       y < (food[i][1]+food[i][3])) {
      clear_value(food[i][0], food[i][1], food[i][3], food[i][2], 6, gamefield);
      score = score + (food[i][4]*speed);
      if(i == 0) {
        game_generate_food_regular();
      }
    }
  }
}

uint8_t game_collisiondetect(void){

  uint8_t collision = 0;

  uint8_t x = snake[0];
  uint8_t y = snake[1];
  uint8_t xr = x + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : x + 1;
  uint8_t yd = y + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : y + 1;
  uint8_t xl = x - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : x - 1;
  uint8_t yu = y - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : y - 1;

  uint8_t cx = 0, cy = 0;

  // dessa är gemensamma för båda fallen
  if(gamefield[y][x] != 0){
    cy = y;
    cx = x;
    collision = gamefield[cy][cx];
  }
  else if(gamefield[y][xr] != 0){
    cy = y;
    cx = xr;
    collision = gamefield[cy][cx];
  }
  else if(gamefield[yd][x] != 0){
    cy = yd;
    cx = x;
    collision = gamefield[cy][cx];
  }
  else if(gamefield[yd][xr] != 0){
    cy = yd;
    cx = xr;
    collision = gamefield[cy][cx];
  }

  if(snake[2] == 2 || snake[2] == 5){
    if(gamefield[yu][x] != 0){
      cy = yu;
      cx = x;
      collision = gamefield[cy][cx];
    }
    else if(gamefield[yu][xr] != 0){
      cx = xr;
      cy = yu;
      collision = gamefield[cy][cx];
    }
  }
  if(snake[2] == 3 || snake[2] == 4){
    if(gamefield[y][xl] != 0){
      cx = xl;
      cy = y;
      collision = gamefield[cy][cx];
    }
    else if(gamefield[yd][xl] != 0){
      cx = xl;
      cy = yd;
      collision = gamefield[cy][cx];
    }
  }

  if(collision >= 6){
    game_collision(cx, cy);
  }
  else if(collision > 0){
    game_end_init(score);
    gamestate = GAME_END;
  }

  seed = (unsigned)TMR2;

  return collision;
}

// The snake moves forward
void game_move(void){
  uint8_t fliphead = 0, collision = 0;

  uint8_t x = snake[0];
  uint8_t y = snake[1];

  // Variables for safely crossing edge of screen with head
  uint8_t xl1 = x - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : x - 1;
  uint8_t xl2 = xl1 - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : xl1 - 1;
  uint8_t xl3 = xl2 - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : xl2 - 1;

  uint8_t xr1 = x + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : x + 1;
  uint8_t xr2 = xr1 + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : xr1 + 1;
  uint8_t xr3 = xr2 + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : xr2 + 1;

  uint8_t yu1 = y - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : y - 1;
  uint8_t yu2 = yu1 - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : yu1 - 1;
  uint8_t yu3 = yu2 - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : yu2 - 1;

  uint8_t yd1 = y + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : y + 1;
  uint8_t yd2 = yd1 + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : yd1 + 1;
  uint8_t yd3 = yd2 + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : yd2 + 1;

  // Update direction bit
  insert_square(snake[0], snake[1], 1, 1, snake[2], gamefield);

  // Update position and old head
  switch(snake[2]) {
    case 2: // Up
      if(snake[6] != snake[2]) {
        if(snake[6] == 3) { // Previously right
          // Remove head bit
          insert_square(xr1, y, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xr1 > xr2 ? xr2 : xr1;
          snake[1] = yu1;
        }
        else if(snake[6] == 4) { // Previously left
          // Remove head bit
          insert_square(xl1, y, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xl2 > xl1 ? xl3 : xl2;
          snake[1] = yu1;
          fliphead = 1;
        }
      }
      else {
        // Remove head bit
        insert_square(xr1, yu1, 1, 1, 0, gamefield);

        // Update position
        snake[1] = yu3;
      }
      break;
    case 3: // Right
      if(snake[6] != snake[2]) {
        if(snake[6] == 2) { // Previously up
          // Remove head bit
          insert_square(xr1, yu1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xr2;
          snake[1] = yu2 > yu1 ? yu3 : yu2;
          fliphead = 1;
        }
        else if(snake[6] == 5) { // Previously down
          // Remove head bit
          insert_square(xr1, yd1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xr2;
          snake[1] = yd1 > yd2 ? yd2 : yd1;
        }
      }
      else {
        // Remove head bit
        insert_square(xr1, yd1, 1, 1, 0, gamefield);

        // Update position
        snake[0] = xr3;
      }
      break;
    case 4: // Left
      if(snake[6] != snake[2]) {
        if(snake[6] == 2) { // Previously up
          // Remove head bit
          insert_square(x, yu1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xl1;
          snake[1] = yu2 > yu1 ? yu3 : yu2;
          fliphead = 1;
        }
        else if(snake[6] == 5) {  // Previously down
          // Remove head bit
          insert_square(x, yd1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xl1;
          snake[1] = yd1 > yd2 ? yd2 : yd1;
        }
      }
      else {
        // Remove head bit
        insert_square(xl1, yd1, 1, 1, 0, gamefield);

        // Update position
        snake[0] = xl3;
      }
      break;
    case 5: // Down
      if(snake[6] != snake[2]) {
        if(snake[6] == 3) { // Previously right
          // Remove head bit
          insert_square(xr1, yd1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xr1 > xr2 ? xr2 : xr1;
          snake[1] = yd2;
        }
        if(snake[6] == 4) { // Previously left
          // Remove head bit
          insert_square(xl1, yd1, 1, 1, 0, gamefield);

          // Update position
          snake[0] = xl2 > xl1 ? xl3 : xl2;
          snake[1] = yd2;
          fliphead = 1;
        }
      }
      else {
        // Remove head bit
        insert_square(xr1, yd1, 1, 1, 0, gamefield);

        // Update position
        snake[1] = yd3;
      }
      break;
  }

  collision = game_collisiondetect();

  if(!collision) {

    x = snake[3];
    y = snake[4];

    // Variables for safely crossing edge of screen with head
    xl1 = x - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : x - 1;
    xl2 = xl1 - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : xl1 - 1;
    xl3 = xl2 - 1 < GAMEFIELD_LEFT ? GAMEFIELD_LEFT + GAMEFIELD_WIDTH - 1 : xl2 - 1;

    xr1 = x + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : x + 1;
    xr2 = xr1 + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : xr1 + 1;
    xr3 = xr2 + 1 >= GAMEFIELD_LEFT + GAMEFIELD_WIDTH ? GAMEFIELD_LEFT : xr2 + 1;

    yu1 = y - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : y - 1;
    yu2 = yu1 - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : yu1 - 1;
    yu3 = yu2 - 1 < GAMEFIELD_TOP ? GAMEFIELD_TOP + GAMEFIELD_HEIGHT - 1 : yu2 - 1;

    yd1 = y + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : y + 1;
    yd2 = yd1 + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : yd1 + 1;
    yd3 = yd2 + 1 >= GAMEFIELD_TOP + GAMEFIELD_HEIGHT ? GAMEFIELD_TOP : yd2 + 1;

    // Save next turn
    uint8_t next = gamefield[y][x];

    // Remove tail
    if(snake[5] == 2 || snake[5] == 5) {
      insert_square(x, yu1, 1, 1, 0, gamefield);
      insert_square(x, y, 1, 1, 0, gamefield);
      insert_square(x, yd1, 1, 1, 0, gamefield);
      insert_square(xr1, yu1, 1, 1, 0, gamefield);
      insert_square(xr1, y, 1, 1, 0, gamefield);
      insert_square(xr1, yd1, 1, 1, 0, gamefield);
    }
    else if(snake[5] == 3 || snake[5] == 4) {
      insert_square(xl1, y, 1, 1, 0, gamefield);
      insert_square(xl1, yd1, 1, 1, 0, gamefield);
      insert_square(x, y, 1, 1, 0, gamefield);
      insert_square(x, yd1, 1, 1, 0, gamefield);
      insert_square(xr1, y, 1, 1, 0, gamefield);
      insert_square(xr1, yd1, 1, 1, 0, gamefield);
    }

    // Update position of tail
    switch(next) {
      case 2: // Up
        if(snake[5] == 3) { // Previously right
          snake[3] = xr1 > xr2 ? xr2 : xr1;
          snake[4] = yu1;
        }
        else if(snake[5] == 4) {  // Previously left
          snake[3] = xl2 > xl1 ? xl3 : xl2;
          snake[4] = yu1;
        }
        else {  // Previously up
          snake[4] = yu3;
        }
        break;
      case 3: // Right
        if(snake[5] == 2) { // Previously up
          snake[3] = xr2;
          snake[4] = yu2 > yu1 ? yu3 : yu2;
        }
        else if(snake[5] == 5) { // Previously down
          snake[3] = xr2;
          snake[4] = yd1 > yd2 ? yd2 : yd1;
        }
        else {  // Previously right
          snake[3] = xr3;
        }
        break;
      case 4: // Left
        if(snake[5] == 2) { // Previously up
          snake[3] = xl1;
          snake[4] = yu2 > yu1 ? yu3 : yu2;
        }
        else if(snake[5] == 5) { // Previously down
          snake[3] = xl1;
          snake[4] = yd1 > yd2 ? yd2 : yd1;
        }
        else {  // Previously left
          snake[3] = xl3;
        }
        break;
      case 5: // Down
        if(snake[5] == 3) { // Previously right
          snake[3] = xr1 > xr2 ? xr2 : xr1;
          snake[4] = yd2;
        }
        else if(snake[5] == 4) { // Previously left
          snake[3] = xl2 > xl1 ? xl3 : xl2;
          snake[4] = yd2;
        }
        else {  // Previously
          snake[4] = yd3;
        }
        break;
    }
    snake[5] = next;
  }

  // Copy direction to previous direction
  snake[6] = snake[2];

  // Draw new head
  draw_head(snake[0], snake[1], snake[2], fliphead);

  // Draw score
  insert_num(23, 26, score, gamefield, 0);

  counter = (counter + 1) % 30;
}

/* Draw game */
void game_draw(void) {
    display_full_bin(gamefield);
}
