#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>

void SysTick_Init(void){
		NVIC_ST_CTRL_R = 0; 
		NVIC_ST_RELOAD_R = 0x00FFFFFF; 
		NVIC_ST_CURRENT_R = 0;  
		NVIC_ST_CTRL_R = 0x00000005;
}
// The delay parameter is in units of the 80 MHz core clock(12.5 ns)
void SysTick_Wait(uint32_t delay){
		NVIC_ST_RELOAD_R = delay-1; 
		NVIC_ST_CURRENT_R = 0;

		while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for flag
		}
}
// Call this routine to wait for delay*10ms 
void SysTick_Wait10ms(uint32_t delay){ 
		unsigned long i;
		for(i=0; i<delay; i++){ 
		SysTick_Wait(800000); 
		}
}