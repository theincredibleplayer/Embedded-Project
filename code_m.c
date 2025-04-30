#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "UART_CONFIG.h"
#include "parsing.h"
#include "GPIO_Systick.h"


int main(void)
{
		UART5_Init();
		UART0_Init();
		PORTB_Init();
		
		//UART5_ReceiveChar();
		//GPS_ReadData();
		//GPS_list();

    return 0;
}

