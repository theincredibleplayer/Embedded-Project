#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include "uart_c.h"

void UART3_Init(void) {
    SYSCTL_RCGCUART_R |= 0x08;       // Enable UART3 clock
    SYSCTL_RCGCGPIO_R |= 0x04;       // Enable Port C clock
    while((SYSCTL_PRGPIO_R & 0x04) == 0); // Wait for Port C ready

    GPIO_PORTC_AFSEL_R |= 0xC0;      // PC6, PC7 alternate function
    GPIO_PORTC_PCTL_R &= ~0xFF000000;
    GPIO_PORTC_PCTL_R |= 0x11000000; // PC6 = RX, PC7 = TX
    GPIO_PORTC_DEN_R |= 0xC0;        // Enable digital I/O on PC6, PC7
    GPIO_PORTC_DIR_R |= 0x80;        // PC7 output, PC6 input

    UART3_CTL_R &= ~0x01;            // Disable UART3
    UART3_IBRD_R = 104;              // 9600 baud
    UART3_FBRD_R = 11;
    UART3_LCRH_R = 0x70;             // 8-bit, no parity, 1 stop bit
    UART3_CTL_R = 0x301;             // Enable UART3
}

void UART3_Send(uint8_t data) {
    while((UART3_FR_R & 0x20) != 0); // Wait for TX buffer not full
    UART3_DR_R = data;
}
