#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "UART_CONFIG.h"

//hello


int main(void)
{
		UART5_Init();
		UART0_Init();
		PORTB_Init();
		UART5_ReceiveChar();
	  // no having fun in this repo >:(
    //'hello world :)'''
	//hello once more
    return 0;
}

