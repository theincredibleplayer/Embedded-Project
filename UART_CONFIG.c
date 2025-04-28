#include "UART_CONFIG.h"
#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>

// This is ibrahim's Branch

void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;          // Enable UART0 clock
    SYSCTL_RCGCGPIO_R |= 0x01;          // Enable GPIOA clock

    UART0_CTL_R &= ~UART_CTL_UARTEN;    // Disable UART for config
    UART0_IBRD_R = 104;                 // Baud Rate Integer (16MHz, 9600bps)
    UART0_FBRD_R = 11;                  // Baud Rate Fraction
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);  // 8-bit, FIFO
    UART0_CTL_R |= (UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE);  // Enable UART

    GPIO_PORTA_AFSEL_R |= 0x03;         // PA0, PA1 alternate function
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011; // UART0
    GPIO_PORTA_DEN_R |= 0x03;           // Digital enable PA0, PA1
    GPIO_PORTA_AMSEL_R &= ~0x03;        // Disable analog on PA0, PA1
}

void UART0_SendChar(char c) {
    while((UART0_FR_R & UART_FR_TXFF) != 0);  // Wait until TX buffer not full
    UART0_DR_R = c;
}

void UART0_SendString(const char *str) {
    while(*str) {
        UART0_SendChar(*str++);
    }
}

void UART0_SendNumber(int num) {
    char buffer[10];
    int i = 0;

    if(num == 0) {
        UART0_SendChar('0');
        return;
    }

    if(num < 0) {                       // Handle negative numbers
        UART0_SendChar('-');
        num = -num;
    }

    while(num > 0) {                    // Convert number to string (reverse order)
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while(i--) {                        // Send digits in correct order
        UART0_SendChar(buffer[i]);
    }
}



void UART5_Init(void) {
		SYSCTL_RCGCUART_R |= 0x20;
		SYSCTL_RCGCGPIO_R |= 0x10;
		while ((SYSCTL_PRGPIO_R & 0x10) == 0) ;

		UART5_CTL_R &= ~0x01;      
		GPIO_PORTE_AFSEL_R|= 0x30;
		GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFF00FFFF) | (0x11 << 16);
		GPIO_PORTE_DEN_R |= 0x30;
		UART5_IBRD_R = 104;     // Integer part
		UART5_FBRD_R = 11;      // Fractional part
		UART5_LCRH_R = 0x60;
		UART5_CTL_R |= (UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN);
}


char UART5_ReceiveChar(void) {
			while (UART5_FR_R & UART_FR_RXFE){
			}
			UART0_SendChar((char)(UART5_DR_R & 0xFF));
    return (char)(UART5_DR_R & 0xFF);
}

char *UART5_ReceiveString(char *string) {
		int i=0;
		while((char)(UART5_DR_R & 0xFF) != '\n'){
			while (UART5_FR_R & UART_FR_RXFE){
			}
			*(string + i++) = (char)(UART5_DR_R & 0xFF);
			UART0_SendChar((char)(UART5_DR_R & 0xFF));
		}
    return string;
}

void PORTB_Init(void){
	  SYSCTL_RCGCGPIO_R |= 0x02;
    while((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) == 0) { }
    GPIO_PORTB_DIR_R |= 0x3F;
    GPIO_PORTB_DEN_R |= 0x3F;
}