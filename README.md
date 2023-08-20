# IS1200_Snake_Game

## Course 
Computer Hardware Engineering, IS1200 at KTH Kista. The course was attended during spring 2023, and the project was presented in December 2023.

## Projekt
The project is developed in the C language on a ChipKIT Uno32 board together with a ChipKIT Basic I/O. A classic snake game was chosen to be developed.

### How to play the game ?
To start the game, you need to pull the fourth switch (SW4) up. Thereafter, a white rectangle will be displayed on the screen, which will represent the snake. The snake can be controlled by the four buttons on the ChipKIT board - where BTN1 moves it right, BTN2 moves it left, BTN3 moves it up, and BTN4 moves it down. The main goal of the game is to chase the apples, which will be shown in random places on the screen. Each eaten apple gives you ONE point! The game ends if the snake collides with itself or the wall (edge of the screen). To restart the game after "Game Over" you need to make sure that the SW4 is down then pull it up again! 


### How to install the game ?
To run the game on your ChipKIT, follow these steps:

1. Download the game repository as a ZIP folder and extract the files.
2. Ensure you have the MCB32TOOLS - MSYS Shell installed on your computer to connect and send files to the ChipKIT.
3. Open the MSYS Shell (bash) and navigate to the folder containing the game's source code.
4. Enter the cross-compile environment by running the following command:
```
. /opt/mcb32tools/environment
```
6. Connect your ChipKIT device to your computer.
7. Compile the code using the ``` make ``` command.
8. Install the code on your ChipKIT device with the ``` make install TTYDEV=/dev/ttySX ``` command. Replace the 'X' in /dev/ttySX with the COM number of your device (e.g., /dev/ttyS4).

## Game Files
The same files from LAB 3 were used as the foundation for this snake game. However, to create a fully functional game, only four of the files were modified to include additional necessary functions. Below are the details of these files:

#### mipslabfunc.c 
This files contains essential functions for interacting with the OLED display and supporting the snake game implementation. It includes functions for clearing the display, writing pixels to the display buffer, handling button inputs, checking collisions, generating pseudo-random numbers, and initializing the snake game such as `handle_buttons()`, `handle_food_collision()` and `check_collision()`. 

#### mipslab.h 
This header file contains function declarations, constant definitions, and global variable declarations used throughout the project. It provides the necessary information for other files to use the functions and variables defined in `mipslabfunc.c` and other files.

#### mipslabwork.c
This file is the main implementation file for the snake game. It contains the `setUp()` function, which is the entry point of the game. The `setUp()` function initializes the OLED display, sets up the game loop, and calls other functions to set up the game environment. It includes calls to functions like `init_snake()` to initialize the snake and `move_snake()` to move the snake according to the player's input.

#### mipslabmain.c 
This file is the main driver file that sets up the hardware and initializes the game. It contains the `main()` function, which initializes the PIC32 microcontroller, sets up the game environment, and calls the `setUp()` function from `mipslabwork.c` to start the game.
