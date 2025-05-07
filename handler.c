#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "UART_CONFIG.h"
#include "parsing.h"
#include "GPIO_Systick.h"
#include "LCD.h"

void UART2_Handler(void){
    char i = 0;
    UART2_ICR_R |= UART_ICR_RXIC ;
    GPS_ReadData();
	  GPS_list();
    UART0_SendNewLine();
    UART0_SendNumberFloat(My_Longitude,4);
    UART0_SendNewLine();
    UART0_SendNumberFloat(My_Latitude,4);
    UART0_SendNewLine();
    Distance();
    for(i=0;i<5;i++){
            UART0_SendNumberFloat(Distance_Arr[i],4);
            UART0_SendNewLine();
    }  

}


void UART5_Handler(void){
    char i = 0;
    UART5_ICR_R |= UART_ICR_RXIC;
    GPS_ReadData();
	GPS_list();
    UART0_SendNewLine();
    UART0_SendNumberFloat(My_Longitude,4);
    UART0_SendNewLine();
    UART0_SendNumberFloat(My_Latitude,4);
    UART0_SendNewLine();
    Distance();
    for(i=0;i<5;i++){
            UART0_SendNumberFloat(Distance_Arr[i],4);
            UART0_SendNewLine();
    }  
}


