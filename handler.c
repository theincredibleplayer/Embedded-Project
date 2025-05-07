#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "UART_CONFIG.h"
#include "parsing.h"
#include "GPIO_Systick.h"
#include "LCD.h"

void UART2_Handler(void){

}


void UART5_Handler(void){
    char i = 0;
    UART5_ICR_R |= UART_ICR_RXIC;
    GPS_ReadData();
		GPS_list();
	  Distance();

    UART0_SendNewLine();
    UART0_SendNumberFloat(My_Longitude,4);
    UART0_SendNewLine();
    UART0_SendNumberFloat(My_Latitude,4);
    UART0_SendNewLine();
		UART0_SendString(hours_str);
		UART0_SendChar(':');
		UART0_SendString(minutes_str);
		UART0_SendChar(':');
		UART0_SendString(seconds_str);
    UART0_SendNewLine();

    for(i=0;i<5;i++){
            UART0_SendNumberFloat(Distance_Arr[i],4);
            UART0_SendNewLine();
    }
}


