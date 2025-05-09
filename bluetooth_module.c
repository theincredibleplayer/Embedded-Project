#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>
#include "TM4C123.h"
#include "bluetooth_module.h"
#include "parsing.h"
#include "audio.h"

void Bluetooth(){
		
		char c;
		if (!(UART7->FR & (1 << 4))) {  // If the RX FIFO is not empty, data is available
        c = Bluetooth_Read();
		}
		SYSCTL->RCGCGPIO |= 0x20;   /* enable clock to GPIOF */
    GPIOF->DIR |= 0x0E;         //set PF1, PF2 and PF3 as digital output pin
    GPIOF->DEN |= 0x0E;         // CON PF1, PF2 and PF3 as digital GPIO pins
  	Delay(10); 
		
		

	
		
	
		// Check the received character and take action to control onboard LEDs of TM4C123
		// Send status string to Andriod app after turning on/off LEDs 

	          // get a character from UART7
        if( c=='A'){
			Bluetooth_Write_String("RED LED ON\n");
			GPIOF->DATA |=(1<<1);
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
		if( c=='G'){
			mute();
			Bluetooth_Write_String("Muted\n");
		}
		else if( c=='H'){
			unmute();
			Bluetooth_Write_String("Unmuted\n");
		}
	
}

void UART7_Init(void){
		////////////////////////////////////////////////////////////////////////////////////
						// 1) Enable clocks to UART7 and Port E
			SYSCTL_RCGCUART_R |= 0x00000080;   // enable UART7 clock (bit 7):contentReference[oaicite:6]{index=6}
			SYSCTL_RCGCGPIO_R |= 0x00000010;   // enable Port E clock (bit 4):contentReference[oaicite:7]{index=7}

			// (Optional) Wait for peripherals to be ready (can poll PRUART/PRGPIO if needed)
			 while((SYSCTL_PRUART_R & 0x80) == 0){}; 
			 while((SYSCTL_PRGPIO_R & 0x10) == 0){};

			// 2) Disable UART7 during configuration
			UART7_CTL_R &= ~UART_CTL_UARTEN;    // UARTCTL.UARTEN = 0

			// 3) Set baud rate: IBRD and FBRD (example: 50 MHz, 9600 baud => 325.52)
			//     integer part = 325, fractional = round(0.5208*64) = 33
			UART7_IBRD_R = 104;                // set integer baud rate divisor
			UART7_FBRD_R = 11;                 // set fractional baud rate divisor

			// 4) Line control: 8 data bits, no parity, 1 stop bit, enable FIFO
			UART7_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN; // (0x60 | 0x10) = 0x70

			// 5) UART clock source: use system clock (default)
			//     (optional since default UARTCC = 0 selects system clock)
			UART7_CC_R = 0x0;                  // UARTCC = 0 (system clock)

			// 6) Enable UART7 TX and RX
			UART7_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE);  // enable receiver and transmitter

			// 7) Re-enable UART7
			UART7_CTL_R |= UART_CTL_UARTEN;    // UARTCTL.UARTEN = 1

			// 8) GPIO Port E configuration for PE0 (RX) and PE1 (TX)
			//    - disable analog, enable alt funct, set PCTL, enable digital, set direction
			GPIO_PORTE_AMSEL_R &= ~0x03;       // disable analog on PE0, PE1
			GPIO_PORTE_AFSEL_R |= 0x03;        // enable alternate function on PE0, PE1

			// Configure PCTL for UART7: set PMCx = 0x1 for PE0 and PE1 (bits [7:0] = 0x11):contentReference[oaicite:8]{index=8}
			GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & ~0x000000FF) | 0x00000011;

			GPIO_PORTE_DEN_R |= 0x03;          // enable digital I/O on PE0, PE1

			// Set PE1 as output (TX) and PE0 as input (RX)
			GPIO_PORTE_DIR_R = (GPIO_PORTE_DIR_R & ~0x01) | 0x02;
	
			/*//interrupt
			// 1) Enable UART7 receive interrupt (set RXIM bit in UARTIM)
			UART7_IM_R |= (1 << 4);    // RXIM = 1 enables receive interrupts:contentReference[oaicite:1]{index=1}

			// (Optionally clear any prior RX interrupt)
			UART7_ICR_R = (1 << 4);    // clear pending RX interrupt (RXIC):contentReference[oaicite:2]{index=2}
			
			// 2) Enable UART7 interrupt in NVIC
			//NVIC_EN2_R |= (1 << 15);    // Enable IRQ79 (UART7) in NVIC:contentReference[oaicite:6]{index=6}
			NVIC_PRI15_R = (NVIC_PRI15_R & ~0x0000E000) | (2 << 13);  // Clear IRQ61 priority bits (mask 0xE000) and set priority = 2 (shift into bits 15–13):contentReference[oaicite:1]{index=1}.
			NVIC_EN1_R |= (1 << (61 - 32));  // Enable IRQ61 (UART7) in NVIC: set bit 29 (61-32) of NVIC_EN1_R:contentReference[oaicite:3]{index=3}.
			*/
	//////////////////////////////////////////////////////////////////////////////////////////////
		
}

char Bluetooth_Read(void)  
{
    char data;
	  while((UART7->FR & (1<<4)) != 0); /* wait until Rx buffer is not full */
    data = UART7->DR ;  	/* before giving it another byte */
    return (unsigned char) data;
}

void Bluetooth_Write(unsigned char data)  
{
    while((UART7->FR & (1<<5)) != 0); /* wait until Tx buffer not full */
    UART7->DR = data;                  /* before giving it another byte */
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
	