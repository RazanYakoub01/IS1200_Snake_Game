/* mipslab.h
   The file is modified by : Razan Yakoub
*/

#include <stdbool.h> // Include this to use the 'bool' type

/* Declare display-related functions from mipslabfunc.c */

void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
void quicksleep(int cyc);
void display_clear();

/* Declare text buffer for display output */
extern char textbuffer[4][16];
/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];

/////////////////////////////////////////////////////////////////////////// 

/* Here follows my own declarations */

//Define display buffer
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 32
uint8_t display_state[DISPLAY_HEIGHT][DISPLAY_WIDTH]; // Declare display buffer for graphic display output

#define MAX_SNAKE_LENGTH 120
#define food_size 2

// Arrays to store the x and y coordinates of the snake's body segments
int snake_x[MAX_SNAKE_LENGTH];
int snake_y[MAX_SNAKE_LENGTH];

// Variables to keep track of the snake's length and direction
int snake_length;
int dx, dy; // Direction of movement

int score;

int food_x;
int food_y;

// Variables for the random function
extern int frame_count;
extern int seed1; // for the first number
extern int seed2; // for the second number

extern int btns;

// Declarations of functions for the logic of the game
int getbtns();
int getsw();
void display_clear(void);
void oled_clear();
void set_pixel(int x, int y);
void display_buffer_to_oled();
void init_snake();
void move_snake();
void handle_buttons();
bool check_collision();
void handle_food_collision(int *food_x, int *food_y);
int pseudo_random_1(void);
int pseudo_random_2(void);
void display_score();
