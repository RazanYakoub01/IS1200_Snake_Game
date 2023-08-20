/* mipslabfunc.c
The file is modified by : Razan Yakoub 
*/

#include <stdlib.h>
#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"
#include <stdio.h>
#include <stdbool.h> // Include this to use the 'bool' type

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/*This function is a part from LAB 3 - NOT MY OWN CODE*/
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/*This function is a part from LAB 3 - NOT MY OWN CODE*/
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

/*This function is a part from LAB 3 - NOT MY OWN CODE*/
void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

/*This function is a part from LAB 3 - NOT MY OWN CODE*/
void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;
	
	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

/*This function is a part from LAB 3 - NOT MY OWN CODE*/
void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);
		
		spi_send_recv(0x0);
		spi_send_recv(0x10);
		
		DISPLAY_CHANGE_TO_DATA_MODE;
		
		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;
			
			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Here follows my own functions */

// Function to clear the screen and make it black
void display_clear(void) {
    int i, j;
    // Clear the textbuffer by setting all characters to spaces (0)
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 16; j++) {
            textbuffer[i][j] = 0;
        }
    }
    // Update the display to clear it
    display_update();
}

// Function to clear the OLED display buffer by setting all pixels to off (0)
void oled_clear() {
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            display_state[y][x] = 0;
        }
    }
}

// Function to set a pixel in the display buffer - This function is used to draw objects pixel by pixel
void set_pixel(int x, int y) {
    if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT) {
        display_state[y][x] =  1 ;
    }
}


// Function to display the contents of the display buffer on the OLED
// A part of this function is taken from LAB 3 - display_update
void display_buffer_to_oled() {
    for (int page = 0; page < DISPLAY_HEIGHT / 8; page++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22); // Set page address
        spi_send_recv(page);

        spi_send_recv(0x00); // Lower 4 bits of column address (0-3)
        spi_send_recv(0x10); // Upper 4 bits of column address (4-7)

        DISPLAY_CHANGE_TO_DATA_MODE;

        for (int col = 0; col < DISPLAY_WIDTH; col++) {
            // Combine 8 pixel states to form a byte
            uint8_t data_byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                int y = page * 8 + bit;
                if (display_state[y][col] != 0) {
                    data_byte |= (1 << bit);
                }
            }
            spi_send_recv(data_byte);
        }
    }
}


// Function to initialize the snake
void init_snake() {
    // Set the initial position and length of the snake
    int x = 64; // To start in the middle of the screen - 128 x-coordinates
    int y = 16; // To start in the middle of the screen - 32 y-coordinates
    snake_length = 10;
    dx = 1;
    dy = 0;

    // Create the initial body segments
    for (int i = 0; i < snake_length; i++) {
        snake_x[i] = x - i;
        snake_y[i] = y;
    }
}

// Function to move the snake in the current direction
void move_snake() {
// Store the previous head coordinates
int prevHeadX = snake_x[0];
int prevHeadY = snake_y[0];

// Move the head in the current direction
snake_x[0] += dx; // dx represents the change in the x-coordinate of the head
snake_y[0] += dy; // dy represents the change in the y-coordinate of the head

// Move the body segments from the tail to the head
for (int i = 1; i < snake_length; i++) {
    // Store the current segment's coordinates
    int tempX = snake_x[i];
    int tempY = snake_y[i];

    // Move the current segment to the position of the previous segment (prevHeadX, prevHeadY)
    snake_x[i] = prevHeadX;
    snake_y[i] = prevHeadY;

    // Update prevHeadX and prevHeadY for the next iteration
    prevHeadX = tempX;
    prevHeadY = tempY;
    }
}

// Function to handle button interactions and change the snake's direction
void handle_buttons() {
    int btns = getbtns();

    // Define an array to map button states to direction changes (dx, dy)
    int directions[4][2] = {
        {-1, 0}, // move left - BTN2
        {0, -1}, // move up - BTN3
        {0, 1},  // move down - BTN4
        {1, 0}   // move right - BTN1
    };

    // Find the pressed button (if any)
    int pressed_button = 0;
    while (btns) {
        btns >>= 1; // shifts value one step right
        pressed_button++; // Increments the pressed_button variable
                         // BTN1 = 1000 gives p_b = 4 , BTN2 = 0001 gives p_b = 1 , BTN3 = 0010 gives p_b = 2 and BTN4 = 0100 gives p_b = 3
    }

    // If a button is pressed and the direction is not opposite, change the direction accordingly
    if (pressed_button > 0 && pressed_button <= 4) {
        int new_dx = directions[pressed_button - 1][0];
        int new_dy = directions[pressed_button - 1][1];

        // Check if the new direction is opposite to the current direction
        if (new_dx != -dx || new_dy != -dy) {
            dx = new_dx;
            dy = new_dy;
        }
    }
}

// Function to check collision with wall and snake-body
bool check_collision() {
    // Check if the head of the snake collides with the screen bounds
    if (snake_x[0] >= DISPLAY_WIDTH || snake_x[0] <= 0 || snake_y[0] >= DISPLAY_HEIGHT || snake_y[0] <= 0) {
        return true; // Collision with screen bounds detected
    }

    // Check for collisions with the snake's body
    for (int i = 1; i < snake_length; i++) {
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
            return true; // Collision with the body segment
        }
    }
    
    return false; // No collisions
}

// Function to check/handle collision food
void handle_food_collision(int *food_x, int *food_y) {
    // Check if the head of the snake collides with the food
    if (snake_x[0] == *food_x && snake_y[0] == *food_y) {
        // Increase the score and snake length
        score++;
        snake_length++;
        *food_x = pseudo_random_1();
        *food_y = pseudo_random_2();
    }
}

// Function to display the score on the OLED
void display_score() {
    char score_str[2]; // Assumes score won't exceed 99
    score_str[0] = '0' + (score / 10);
    score_str[1] = '0' + (score % 10);
    display_string(2, score_str);
}

// For these function (pseudo_random) we took help from Google , Wikipedia :) So not totaly our own code!
int frame_count = 0;
int seed1 = 1;
int seed2 = 1;
// Function to generate a pseudo-random number between 1 and 127 using seed1
int pseudo_random_1() {
    frame_count++; // Increment the frame count with each frame
    seed1 = (seed1 * 1103515245 + 12345) & 0x7FFFFFFF; // Linear congruential generator formula for seed1
    return (seed1 % 126) + 1;
}

// Function to generate a pseudo-random number between 1 and 31 using seed2
int pseudo_random_2() {
    frame_count++; // Increment the frame count with each frame
    seed2 = (seed2 * 1664525 + 1013904223) & 0x7FFFFFFF; // Linear congruential generator formula for seed2
    return (seed2 % 30) + 1;
}
