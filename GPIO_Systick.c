#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>

// initializes portB for LCD
// initializes ports B0 to B5 as digital outputs
// current physical connections are:
// B0-B3 connect to D4-D7 in the LCD
// B4 is connected to the select register (RS) on the LCD
// B5 is connected to the enable pin on the LCD

void PORTB_Init(void){
	  SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) == 0) { }
    GPIO_PORTB_DIR_R |= 0x3F;
    GPIO_PORTB_DEN_R |= 0x3F;
}