#include "tm4c123gh6pm.h"

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

void sendToDataPort(unsigned char data);  //Maps each bit of the data byte to the correct Port B pin 

void writeLCDData(unsigned char data); //Sends data (a character) to be displayed on the LCD.

void sendLCDCommand(unsigned char command); //Sends a command to the LCD (clear display, set cursor position).

void writeLCDString(char *str, unsigned char length); //Writes a string of given length to the LCD.

void initializeLCD(void); //Sends standard initialization commands to configure the LCD display.

int main() 
{
    // Enable clock for PORTB and PORTD 
    SYSCTL->RCGCGPIO = SYSCTL->RCGCGPIO | (1<<1) | (1<<3);  // PORTB (bit1), PORTD (bit3)

    // Digital enable for all pins
    GPIOB->DEN |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7);  // D0-D7
    GPIOD->DEN |= (1<<0) | (1<<1) | (1<<2);  // RS, RW, EN

    // Set direction for all pins (output)
    GPIOB->DIR |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4) | (1<<5) | (1<<6) | (1<<7);  // D0-D7
    GPIOD->DIR |= (1<<0) | (1<<1) | (1<<2);  // RS, RW, EN
    
    delay_ms(10000);
    initializeLCD();
    sendLCDCommand(0x80); // cursor to first row and first column
    writeLCDString("Ibrahim", 7); // test
    
    while(1)
    {
    }
}

void writeLCDData(unsigned char data)
{
    sendToDataPort(data);
    GPIOD->DATA &= (~1<<1);    // R/W low for write
    GPIOD->DATA |= (1<<0);     // RS high for data
    GPIOD->DATA |= (1<<2);     // EN high
    delay_ms(10000);
    GPIOD->DATA &= (~1<<2);    // EN low
}

void sendLCDCommand(unsigned char command)
{
    sendToDataPort(command);
    GPIOD->DATA &= (~(1<<1));  // R/W low
    GPIOD->DATA &= (~(1<<0));  // RS low for command
    GPIOD->DATA |= (1<<2);     // EN high
    delay_ms(10000);
    GPIOD->DATA &= (~(1<<2));  // EN low
}

void writeLCDString(char *str, unsigned char length) 
{
    char i;
    for(i=0; i<length; i++) 
    {
        writeLCDData(str[i]);
    }
}

void initializeLCD(void)
{
    sendLCDCommand(0x38);    // 8-bit mode, 2 lines, 5x8 font
    sendLCDCommand(0x06);    // Entry mode: increment cursor
    sendLCDCommand(0x0C);    // Display on, cursor off
    sendLCDCommand(0x01);    // Clear display
}

void sendToDataPort(unsigned char data)
{
    // Map each data bit to the correct GPIO pin
    if((data & 0x01) == 0x01) { GPIOB->DATA |= (1<<7); }  // D0
    else { GPIOB->DATA &= (~(1<<7)); }
    
    if((data & 0x02) == 0x02) { GPIOB->DATA |= (1<<6); }  // D1
    else { GPIOB->DATA &= (~(1<<6)); }
    
    if((data & 0x04) == 0x04) { GPIOB->DATA |= (1<<5); }  // D2
    else { GPIOB->DATA &= (~(1<<5)); }
    
    if((data & 0x08) == 0x08) { GPIOB->DATA |= (1<<4); }  // D3
    else { GPIOB->DATA &= (~(1<<4)); }
    
    if((data & 0x10) == 0x10) { GPIOB->DATA |= (1<<3); }  // D4
    else { GPIOB->DATA &= (~(1<<3)); }
    
    if((data & 0x20) == 0x20) { GPIOB->DATA |= (1<<2); }  // D5
    else { GPIOB->DATA &= (~(1<<2)); }
    
    if((data & 0x40) == 0x40) { GPIOB->DATA |= (1<<1); }  // D6
    else { GPIOB->DATA &= (~(1<<1)); }
    
    if((data & 0x80) == 0x80) { GPIOB->DATA |= (1<<0); }  // D7
    else { GPIOB->DATA &= (~(1<<0)); }
}

void delay_ms(long milliseconds)
{ 
    while(milliseconds--);
}