#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include "audio.h"

void UART1_Init(void) {
    SYSCTL_RCGCUART_R |= 0x02;   // Enable UART1
    SYSCTL_RCGCGPIO_R |= 0x02;   // Enable Port B
    while((SYSCTL_PRGPIO_R & 0x02) == 0);

    GPIO_PORTB_AFSEL_R |= 0x03;  // PB0, PB1 alternate function
    GPIO_PORTB_PCTL_R &= ~0x000000FF;
    GPIO_PORTB_PCTL_R |= 0x00000011; // UART1
    GPIO_PORTB_DEN_R |= 0x03;
    GPIO_PORTB_DIR_R |= 0x02;    // PB1 output (TX), PB0 input (RX)

    UART1_CTL_R &= ~0x01;         // Disable UART1
    UART1_IBRD_R = 104;           // 9600 baud (assuming 16 MHz clock)
    UART1_FBRD_R = 11;
    UART1_LCRH_R = 0x70;          // 8-bit, no parity, 1 stop bit, FIFO
    UART1_CTL_R |= 0x301;         // Enable UART1, TXE, RXE
}

void UART1_Transmit(uint8_t data) {
    while((UART1_FR_R & 0x20) != 0);  // Wait until TX buffer not full
    UART1_DR_R = data;
}

void DFPlayer_PlayTrack(uint8_t trackNum) {
    uint8_t command[] = {
        0x7E, 0xFF, 0x06, 0x03, 0x00,
        0x00, trackNum, 0xEF
    };
    for(int i = 0; i < 8; i++) {
        UART1_Transmit(command[i]);
    }
}
