#include "UART_CONFIG.h"
#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "GPIO_Systick.h"
#include <math.h>  

//carpe diem

void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;          // Enable UART0 clock
		while ((SYSCTL_PRUART_R & 0x01) == 0) {}   // Wait for UART0 to be ready

    SYSCTL_RCGCGPIO_R |= 0x01;          // Enable GPIOA clock
		while ((SYSCTL_PRGPIO_R & 0x01) == 0) {}   // Wait for GPIOA to be ready

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

void UART0_SendNumberInt(int num) {
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
// 4 points of precision
void UART0_SendNumberFloat(float num,uint8_t precision) {
    char buffer[10]="";
    uint8_t i = 0;
		uint8_t k = 0;
		int32_t int_part = (int)num;
		int32_t decimal_part = 0;
    if(num == 0.0) {
        UART0_SendChar('0');
        return;
    }
    if(num < 0.0) {                       // Handle negative numbers
        UART0_SendChar('-');
        num = -num;
    }
		int_part = (int)num;																		//split the number into an int part and a decimal part
		decimal_part = (num*pow(10, precision)) - (int_part*pow(10, precision));			//
		
		for(k=0;k<precision;k++){                    // convert decimal part to string (order is reversed), made using a for loop for padding
				buffer[i++] = (decimal_part % 10) + '0';
				decimal_part /= 10;
		}
		
		buffer[i++] = '.'; //decimal point
		
	  if(int_part == 0) {
				buffer[i++] = '0';
    }else{
				while(int_part > 0) {                  // convert int part to string (order is reversed)
						buffer[i++] = (int_part % 10) + '0';
						int_part /= 10;
				}
		}
    while(i--) {                        // Send digits in correct order
        UART0_SendChar(buffer[i]);
    }
}
void UART0_SendNewLine(){
		UART0_SendChar('\r');
		UART0_SendChar('\n');
}

//***************************************************
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
	
		UART5_IM-R |= UART_IM_RXIM;
	  UART5_ICR_R=0X7FF;
		
		NVIC_PRI15_R = (NVIC_PRI15_R & ~0xE0000000) | (1 << 29);  // Priority level 1
		NVIC_EN1_R |= (1 << (61 - 32));      
		EnableInterrupts();
	
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

