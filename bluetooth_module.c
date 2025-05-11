#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>
#include "TM4C123.h"
#include "bluetooth_module.h"
#include "parsing.h"
#include "audio.h"
#include "LCD.h"

void Bluetooth(){
		
		char c = 0;
		if (!(UART7->FR & (1 << 4))) {  // If the RX FIFO is not empty (data is available)
        c = Bluetooth_Read(); // get a character from UART7
		}
		SYSCTL->RCGCGPIO |= 0x20;   /* enable clock to GPIOF */ //for test led
    GPIOF->DIR |= 0x0E;         //set PF1, PF2 and PF3 as digital output pin
    GPIOF->DEN |= 0x0E;         // CON PF1, PF2 and PF3 as digital GPIO pins
  	Delay(10); 
		

	  if( c=='A'){     //for testing when connected
				GPIOF->DATA |=(1<<2);
				Bluetooth_Write_String("BLUE LED ON\n");	//sent through TX7 to the mobile app terminal
		}
		if( c=='B'){
				
				Bluetooth_Write_String("BLUE LED OFF\n");	
		}        
		if( c=='C'){
				write_LCD_Line2_NoClear(Mark_Location(), 16);
				Bluetooth_Write_String("Location marked\n");	//sent through TX7 to the mobile app terminal
		}
		else if( c=='D'){
				Mark_Removal();
				write_LCD_Line2_NoClear("  Mark removed  ", 16);
				Bluetooth_Write_String("Location unmarked\n");	//sent through TX7 to the mobile app terminal
		}
		if( c=='E'){
				set_time_on();
				Bluetooth_Write_String("Time displayed\n");	//sent through TX7 to the mobile app terminal
		}
		if( c=='B'){
				set_speed_on();
				Bluetooth_Write_String("Speed displayed\n");	//sent through TX7 to the mobile app terminal
		}
		if( c=='F'){
				set_distance_on();
				Bluetooth_Write_String("Distance displayed\n");	//sent through TX7 to the mobile app terminal
		}
		if( c=='G'){
				mute();
				Bluetooth_Write_String("Muted\n");	//sent through TX7 to the mobile app terminal
		}
		else if( c=='H'){
				unmute();
				Bluetooth_Write_String("Unmuted\n");	//sent through TX7 to the mobile app terminal
		}
		if( c=='I'){
				set_language_arabic();
				Bluetooth_Write_String("Arabic language set\n");	//sent through TX7 to the mobile app terminal
		}
		else if( c=='J'){
				set_language_english();
				Bluetooth_Write_String("English language set\n");	//sent through TX7 to the mobile app terminal
		}
	
}

void UART7_Init(void){
			// Enable clocks to UART7 and Port E
			SYSCTL_RCGCUART_R |= 0x00000080;   // enable UART7 clock (bit 7)
			SYSCTL_RCGCGPIO_R |= 0x00000010;   // enable Port E clock (bit 4)

			// Wait for peripherals to be ready
			 while((SYSCTL_PRUART_R & 0x80) == 0){}; 
			 while((SYSCTL_PRGPIO_R & 0x10) == 0){};

			// Disable UART7 during configuration
			UART7_CTL_R &= ~UART_CTL_UARTEN;    

			
			UART7_IBRD_R = 104;                // set integer baud rate divisor
			UART7_FBRD_R = 11;                 // set fractional baud rate divisor

			//8 data bits, no parity, 1 stop bit, enable FIFO
			UART7_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; 

			// 5) Use system clock
			UART7_CC_R = 0x0;                  // UARTCC = 0 (system clock)

			// 6) Enable UART7 TX and RX
			UART7_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE);  // enable receiver and transmitter

			// 7) Re-enable UART7
			UART7_CTL_R |= UART_CTL_UARTEN;    

			GPIO_PORTE_AMSEL_R &= ~0x03;       // disable analog on PE0, PE1
			GPIO_PORTE_AFSEL_R |= 0x03;        // enable alternate function on PE0, PE1

			GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & ~0x000000FF) | 0x00000011;

			GPIO_PORTE_DEN_R |= 0x03;          // enable digital I/O on PE0, PE1

			// Set PE1 as output (TX) and PE0 as input (RX)
			GPIO_PORTE_DIR_R = (GPIO_PORTE_DIR_R & ~0x01) | 0x02;
	
		
}

char Bluetooth_Read(void)  
{
    char data;
	  while((UART7->FR & (1<<4)) != 0); // wait until Rx buffer is not full before giving it another byte 
    data = UART7->DR ;  	
    return (unsigned char) data;
}

void Bluetooth_Write(unsigned char data)  
{
    while((UART7->FR & (1<<5)) != 0); // wait until Tx buffer not full before giving it another byte 
    UART7->DR = data;                   
}

void Bluetooth_Write_String(char *str)
{
  while(*str)
	{
		Bluetooth_Write(*(str++));
	}
}
void Delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter; i++);
}
	