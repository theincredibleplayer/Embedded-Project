
#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "UART_CONFIG.h"
#include "parsing.h"
#include "GPIO_Systick.h"
#include "LCD.h"
#include "audio.h"
#include "uart_c.h"
#include "bluetooth_module.h"


int main(void)
{
		uint8_t i = 0;
		UART5_Init();
		UART0_Init();
		//PORTB_Init();
		initialize_LCD_Ports();
		UART3_Init();
		UART2_Init();
		while(1);
    return 0;
}

