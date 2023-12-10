/* mipslabwork.c
   The file is modified by : Razan Yakoub
*/

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <stdbool.h> // Include this to use the 'bool' type


/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/*This is function is called to start the game
From it other functions are called which set the game up
Written by : Razan Yakoub
*/
void setUp()
{
    // Initialize the OLED display
    display_init();

    bool game_over = true; // Set game_over to true initially to start the game

    while (1) { // Infinite loop for restarting the game
        if (game_over) {
            // Wait for switch 4 to be down before starting the game
            while ((getsw() & 0x08) == 0) {
                continue; // Wait for switch 4 to be down
            }
            // Initialize the snake
            create_first_snake();  
            food_x = random_x();
            food_y = random_y();
            game_over = false; // Set game_over to false to start the game
        }


        while (!game_over) { // Keep running the game until game_over becomes true
            // Clear the display buffer and display the cleared buffer on the OLED
            oled_clear();

            // Draw the apples
            for (int i = 0; i < food_size; i++) {
                set_pixel(food_x, food_y);
            }

            // Draw the snake on the display buffer
            for (int i = 0; i < snake_length; i++) {
                set_pixel(snake_x[i], snake_y[i]);
            }

            // Display the updated buffer with the objects (snake and apple) on the OLED
            oled_update();

            // Wait for a while to control the speed of movement
            quicksleep(500000); // Adjust the value for desired speed

            // Handle button interactions and change the snake's direction
            handle_buttons();

            // Move the snake in the current direction
            automate_snake_move();

            // Check for food collision
            handle_food_collision(&food_x,&food_y);

            // Check for collisions
            if (check_collision()) {
                // Display "GAME OVER" message
                oled_clear();
                display_string(0, "GAME OVER");
                display_string(1, " Your Score : ");
                display_score();
                display_string(3, "S: put SW4 up");
                display_update();
                quicksleep(100); 
                score = 0;
                game_over = true; // Set game_over to true to exit the game loop
            }
        }

        // Wait for switch 4 to be up before restarting the game
        while ((getsw() & 0x08) != 0) {
            continue; // Wait for switch 4 to be up
        }
    }

    return 0;
}