#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>
#include "TM4C123.h"
#include "bluetooth_module.h"
#include "parsing.h"

void Bluetooth(){
		/* Set PF1, PF2 and PF3 as digital output pins */
			char c = Bluetooth_Read();          // get a character from UART5 


  	//Delay(10); 
	
		
		// Check the received character and take action to control onboard LEDs of TM4C123
		// Send status string to Andriod app after turning on/off LEDs 

    if( c=='A'){
			GPIOF->DATA |=(1<<1);
			Bluetooth_Write_String("RED LED ON\n");
		}
		else if( c=='B'){
			GPIOF->DATA &=~(1<<1);
			Bluetooth_Write_String("RED LED OFF\n");
		}
		else if( c=='C'){
			GPIOF->DATA |=(1<<2);
			Bluetooth_Write_String("BLUE LED ON\n");
		}
		else if( c=='D'){
			GPIOF->DATA &=~(1<<2);
			Bluetooth_Write_String("BLUE LED OFF\n");
		}
		if( c=='E'){
			GPIOF->DATA |=(1<<3);
			Bluetooth_Write_String("GREEN LED ON\n");
		}
		else if( c=='F'){
			GPIOF->DATA &=~(1<<3);
			Bluetooth_Write_String("GREEN LED OFF\n");
		}
		if( c=='M'){
			Bluetooth_Write_String( Mark_Location());
		}
		else if( c=='U'){
			Mark_Removal();
		}
}

void UART2_Init(void){
    SYSCTL_RCGCUART_R |= 0x04;       // Enable UART2 clock
    SYSCTL_RCGCGPIO_R |= 0x08;       // Enable Port D clock
    while((SYSCTL_PRGPIO_R & 0x08) == 0); // Wait for Port D ready

    GPIO_PORTD_AFSEL_R |= 0xC0;      // PD6, PD7 alternate function
    GPIO_PORTD_PCTL_R &= ~0xFF000000;
    GPIO_PORTD_PCTL_R |= 0x11000000; // PD6 = RX, PD7 = TX
    GPIO_PORTD_DEN_R |= 0xC0;        // Enable digital I/O on PD6, PD7
    GPIO_PORTD_DIR_R |= 0x80;        // PD7 output, PD6 input

    UART2_CTL_R &= ~0x01;            // Disable UART2
    UART2_IBRD_R = 104;              // 9600 baud
    UART2_FBRD_R = 11;
    UART2_LCRH_R = 0x60;             // 8-bit, not parity bit, no FIFO
    UART2_CTL_R = 0x301;             // Enable UART2
	//interrupt******************************************************************
	  UART2_ICR_R = 0x7FF;                 // Clear all UART2 interrupt flags
    UART2_IM_R |= UART_IM_RXIM;          // Enable RX interrupt
    NVIC_PRI12_R = (NVIC_PRI1_R & ~0xFFFF00FF) | (2 << 13); // Priority 2 (bits 31-29)
    NVIC_EN1_R |= (1 << (49 - 32));      // Enable interrupt 33 (UART2) in NVIC
	
	
	
	//leds
			SYSCTL->RCGCGPIO |= 0x20;   /* enable clock to GPIOF */
    GPIOF->DIR |= 0x0E;         //set PF1, PF2 and PF3 as digital output pin
    GPIOF->DEN |= 0x0E;         // CON PF1, PF2 and PF3 as digital GPIO pins
	
}

char Bluetooth_Read(void)  
{
    char data;
	  while((UART2->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART2->DR ;  	/* before giving it another byte */
    return (unsigned char) data; 
}

void Bluetooth_Write(unsigned char data)  
{
    while((UART2->FR & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART2->DR = data;                  /* before giving it another byte */
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
	