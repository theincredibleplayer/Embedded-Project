#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"

// RS = PD0
// RW = PD1
// EN = PD2

// D0 = PB7
// D1 = PB6
// D2 = PB5
// D3 = PB4
// D4 = PB3
// D5 = PB2
// D6 = PB1
// D7 = PB0

void delay_ms(long milliseconds);
void sendToDataPort(unsigned char data);
void writeLCDData(unsigned char data);
void sendLCDCommand(unsigned char command);
void writeLCDString(char *str, unsigned char length);
void initializeLCD(void);

int main() 
{
    SYSCTL_RCGCGPIO_R |= (1<<1) | (1<<3);  // Enable clock for PORTB and PORTD

    GPIO_PORTB_DEN_R |= 0xFF;  // Enable digital function for PB0-PB7
    GPIO_PORTD_DEN_R |= 0x07;  // Enable digital function for PD0-PD2

    GPIO_PORTB_DIR_R |= 0xFF;  // Set PB0-PB7 as output
    GPIO_PORTD_DIR_R |= 0x07;  // Set PD0-PD2 as output

    delay_ms(10000);
    initializeLCD();
    sendLCDCommand(0x80);  // Move cursor to first row, first column
    writeLCDString("Ibrahim", 7);

    while(1) {}
}

void writeLCDData(unsigned char data)
{
    sendToDataPort(data);
    GPIO_PORTD_DATA_R &= ~(1<<1);  // RW = 0 (write)
    GPIO_PORTD_DATA_R |=  (1<<0);  // RS = 1 (data)
    GPIO_PORTD_DATA_R |=  (1<<2);  // EN = 1
    delay_ms(10000);
    GPIO_PORTD_DATA_R &= ~(1<<2);  // EN = 0
}

void sendLCDCommand(unsigned char command)
{
    sendToDataPort(command);
    GPIO_PORTD_DATA_R &= ~(1<<1);  // RW = 0
    GPIO_PORTD_DATA_R &= ~(1<<0);  // RS = 0 (command)
    GPIO_PORTD_DATA_R |=  (1<<2);  // EN = 1
    delay_ms(10000);
    GPIO_PORTD_DATA_R &= ~(1<<2);  // EN = 0
}

void writeLCDString(char *str, unsigned char length)
{
	char i;
    for( i = 0; i < length; i++) {
        writeLCDData(str[i]);
    }
}

void initializeLCD(void)
{
    sendLCDCommand(0x38);  // 8-bit, 2 line, 5x8 font
    sendLCDCommand(0x06);  // Increment cursor
    sendLCDCommand(0x0C);  // Display on, cursor off
    sendLCDCommand(0x01);  // Clear display
}

void sendToDataPort(unsigned char data)
{
    // Mapping data bits to PB7-PB0
    if(data & 0x01) GPIO_PORTB_DATA_R |= (1<<7);
   	else GPIO_PORTB_DATA_R &= ~(1<<7);
    if(data & 0x02) GPIO_PORTB_DATA_R |= (1<<6);
  	else GPIO_PORTB_DATA_R &= ~(1<<6);
    if(data & 0x04) GPIO_PORTB_DATA_R |= (1<<5);
  	else GPIO_PORTB_DATA_R &= ~(1<<5);
    if(data & 0x08) GPIO_PORTB_DATA_R |= (1<<4);
  	else GPIO_PORTB_DATA_R &= ~(1<<4);
    if(data & 0x10) GPIO_PORTB_DATA_R |= (1<<3);
  	else GPIO_PORTB_DATA_R &= ~(1<<3);
    if(data & 0x20) GPIO_PORTB_DATA_R |= (1<<2); 
	  else GPIO_PORTB_DATA_R &= ~(1<<2);
    if(data & 0x40) GPIO_PORTB_DATA_R |= (1<<1); 
	  else GPIO_PORTB_DATA_R &= ~(1<<1);
    if(data & 0x80) GPIO_PORTB_DATA_R |= (1<<0); 
	  else GPIO_PORTB_DATA_R &= ~(1<<0);
}

void delay_ms(long milliseconds)
{
    while(milliseconds--);
}
