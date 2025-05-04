#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "LCD.h"
// RS = PD0 ; RW = PD1 ; EN = PD2 ;  D0 = PB7 ; D1 = PB6 ; D2 = PB5 ; D3 = PB4 ; D4 = PB3;  D5 = PB2;  D6 = PB1;  D7 = PB0



void initialize_LCD_Ports(void)
{
    SYSCTL_RCGCGPIO_R |= (1<<1) | (1<<3);  // Enable clock for PORTB and PORTD

    GPIO_PORTB_DEN_R |= 0xFF;  // Enable digital function for PB0-PB7
    GPIO_PORTD_DEN_R |= 0x07;  // Enable digital function for PD0-PD2

    GPIO_PORTB_DIR_R |= 0xFF;  // Set PB0-PB7 as output
    GPIO_PORTD_DIR_R |= 0x07;  // Set PD0-PD2 as output

    delay_ms(10000);
    initialize_LCD();
    send_LCD_Command(0x80);  // Move cursor to first row, first column
    write_LCD_String("Ibrahim", 7);

    while(1) {
		}
}

void write_LCD_Data(unsigned char data)
{
    send_To_Data_Port(data);
    GPIO_PORTD_DATA_R &= ~(1<<1);  // RW = 0 (write)
    GPIO_PORTD_DATA_R |=  (1<<0);  // RS = 1 (data)
    GPIO_PORTD_DATA_R |=  (1<<2);  // EN = 1
    delay_ms(10000);
    GPIO_PORTD_DATA_R &= ~(1<<2);  // EN = 0
}

void send_LCD_Command(unsigned char command)
{
    send_To_Data_Port(command);
    GPIO_PORTD_DATA_R &= ~(1<<1);  // RW = 0
    GPIO_PORTD_DATA_R &= ~(1<<0);  // RS = 0 (command)
    GPIO_PORTD_DATA_R |=  (1<<2);  // EN = 1
    delay_ms(10000);
    GPIO_PORTD_DATA_R &= ~(1<<2);  // EN = 0
}

void write_LCD_String(char *str, unsigned char length)
{
	char i;
    for( i = 0; i < length; i++) {
        write_LCD_Data(str[i]);
    }
}

void initialize_LCD(void)
{
    send_LCD_Command(0x38);  
	
    send_LCD_Command(0x06);  
	
    send_LCD_Command(0x0C);  
	
    send_LCD_Command(0x01);  
}

void send_To_Data_Port(unsigned char data)
{
    
    if(data & 0x01) 
			GPIO_PORTB_DATA_R |= (1<<7);
	
   	else GPIO_PORTB_DATA_R &= ~(1<<7);
	
    if(data & 0x02)
			GPIO_PORTB_DATA_R |= (1<<6);
  	else GPIO_PORTB_DATA_R &= ~(1<<6);
		
    if(data & 0x04)
			GPIO_PORTB_DATA_R |= (1<<5);
  	else GPIO_PORTB_DATA_R &= ~(1<<5);
		
    if(data & 0x08)
			GPIO_PORTB_DATA_R |= (1<<4);
  	else GPIO_PORTB_DATA_R &= ~(1<<4);
		
    if(data & 0x10)
			GPIO_PORTB_DATA_R |= (1<<3);
  	else GPIO_PORTB_DATA_R &= ~(1<<3);
		
    if(data & 0x20)
			GPIO_PORTB_DATA_R |= (1<<2); 
	  else GPIO_PORTB_DATA_R &= ~(1<<2);
		
    if(data & 0x40)
			GPIO_PORTB_DATA_R |= (1<<1); 
	  else GPIO_PORTB_DATA_R &= ~(1<<1);
		
    if(data & 0x80) 
			GPIO_PORTB_DATA_R |= (1<<0); 
	  else GPIO_PORTB_DATA_R &= ~(1<<0);
}

void delay_ms(long milliseconds)
{
    while(milliseconds--);
}
