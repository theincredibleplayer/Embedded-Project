
#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "UART_CONFIG.h"
#include "parsing.h"
#include "GPIO_Systick.h"
#include "LCD.h"
#include "audio.h"
#include "uart_c.h"
#include "bluetooth_module.h"
#include "TM4C123.h"


int main(void)
{
		uint8_t i = 0;
		UART5_Init();
		UART0_Init();
		PORTB_Init();
		initialize_LCD_Ports();
		UART3_Init();
		UART7_Init();
		
		while(1){
			GPS_ReadData();
			GPS_list();
			Distance();
			if(time_flag){
					write_LCD_Line2_NoClear(time_str,8);
			}
			if(distance_flag){
					SendDistanceToLCD(total_distance,4);
			}
			Bluetooth();
			
		}
	//while(1);
    return 0;
}

