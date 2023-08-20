#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int getsw(void){

    int swValue =  (PORTD >> 8) & 0xf;
    return swValue;
}

int getbtns(void){

    int btnValue = 0;
    // Read the state of buttons 4, 3, and 2 from PORTD - X111
    btnValue |= (PORTD >> 5) & 0x7;  // Shift the bits 5 positions to the right 
                                    //and mask out the relevant bits using bitwise AND operator

    
    // Read the state of button BTN1 from PORTF - 1XXX 
    btnValue |= ((PORTF >> 1) & 0x1) << 3;  // Shift the bit 1 position to the right 
                                           // and mask out the bit using bitwise AND operator
                                           // Then, shift it 3 positions to the left 
                                           // to match its position in the combined value
                                    
    return btnValue;  // Return the combined state of the buttons
}
