/* basedecl.h
   Header file for all labs.
   This file written 2016 by Oscar Mattsson
   Some parts are original code written by Axel Isaksson and F Lundevall

   For copyright and licensing, see file COPYING */

#define CENTER 128 / 2

/* Declare gamestate variables */
typedef enum { INTRO, MENU, GAME, GAME_END, HIGHSCORE, SETTINGS, HELP } state;
extern state gamestate;
extern state prevgamestate;
extern uint8_t speed;
extern int seed;

/* Declare menu screens array */
extern uint8_t menufield[32][128];

/* Declare functions from func.c */
void display_full(const uint8_t *data);
void display_init(void);
uint8_t spi_send_recv(uint8_t data);
char * itoaconv(int num);
void quicksleep(int cyc);
void insert_object(int x, int y, int height, int width, const uint8_t src[], uint8_t dest[32][128], int reverse);
int insert_char(int x, int y, char c, uint8_t dest[32][128], int reverse);
int insert_string(int x, int y, const char* s, uint8_t dest[32][128], int reverse);
int insert_num(int x, int y, int n, uint8_t dest[32][128], int reverse);
void insert_square(int x, int y, int height, int width, int value, uint8_t dest[32][128]);
void clear_value(int x, int y, int height, int width, int value, uint8_t dest[32][128]);

/* Declare functions from intro.c */
void intro_init(void);
void intro_update(int* buttons, int* switches);
void intro_draw(void);

/* Declare functions from menu.c */
void menu_init(void);
void menu_update(int* buttons, int* switches);
void menu_draw(void);

/* Declare functions from game.c */
void game_init(void);
void game_update(int* buttons, int* switches);
void game_draw(void);

/* Declare functions from highscore.c */
void highscore_init(void);
void highscore_update(int* buttons, int* switches);
void highscore_draw(void);

/* Declare functions from game_end.c */
void game_end_init(int score);
void game_end_update(int* buttons, int* switches);
void game_end_draw(void);

/* Declare functions from help.c */
void help_init(void);
void help_update(int* buttons, int* switches);
void help_draw(void);

/* Declare functions from settings.c */
void settings_init(void);
void settings_update(int* buttons, int* switches);
void settings_draw(void);

/* Declare bitmap array containing game score pattern */
extern const uint8_t const pattern_score[161];
/* Declare bitmap array containing numbers */
extern const uint8_t const numbers[11][15];
/* Declare bitmap array containing small font */
extern const uint8_t const letters[27][25];
extern const uint8_t const symbol_questionmark[25];
extern const uint8_t const symbol_slash[25];
extern const uint8_t const symbol_mult[9];
/* Declare bitmap arrays for intro graphics */
extern const uint8_t const intro_snake[480];
extern const uint8_t const intro_mouse[560];
/* Declare bitmap arrays containing directional arrows */
extern const uint8_t const switch_up[25];
extern const uint8_t const switch_down[25];
extern const uint8_t const button_left[15];
extern const uint8_t const button_right[15];
extern const uint8_t const button_up[15];
extern const uint8_t const button_down[15];
/* Declare bitmap arrays containing game food items */
extern const uint8_t const food_regular[9];
extern const uint8_t const food_lizard[21];
extern const uint8_t const food_mouse[21];
extern const uint8_t const food_bird[30];
extern const uint8_t const food_spider[32];
extern const uint8_t const food_frog[32];
extern const uint8_t const food_turtle[32];
