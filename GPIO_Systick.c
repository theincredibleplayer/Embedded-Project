#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>


void SysTick_Init(void){
		NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup 
		NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value 
		NVIC_ST_CURRENT_R = 0; // 3) any write to CURRENT clears it 
		NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock
}
void SysTick_Wait(uint32_t delay){
		NVIC_ST_RELOAD_R = delay-1; 		// number of counts
		NVIC_ST_CURRENT_R = 0;		// any value written to CURRENT clears
		while((NVIC_ST_CTRL_R&0x00010000)==0); // wait for flag
}
// Call this routine to wait for delay*10ms 
void SysTick_Wait10ms(uint32_t delay){ 
		unsigned long i;
		for(i=0; i<delay; i++){ 
				SysTick_Wait(800000);
		}
}

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