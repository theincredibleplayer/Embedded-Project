#include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
#include <stdint.h>
#include "LCD.h"

// D4-D7 = PB0-PB3, RS = PB4, EN = PB5

void initialize_LCD_Ports(void)
{
    SYSCTL_RCGCGPIO_R |= (1<<1);  // Enable clock for PORTB
    while((SYSCTL_PRGPIO_R & (1<<1)) == 0);  // Wait for clock to stabilize

    GPIO_PORTB_DEN_R |= 0x3F;     // Enable digital function for PB0-PB5
    GPIO_PORTB_DIR_R |= 0x3F;     // Set PB0-PB5 as output

    delay_ms(10);
    initialize_LCD();
    set_LCD_CursorLine(1);        // Select first line
}

void write_LCD_Data(unsigned char data)
{
    send_To_Data_Port(data >> 4);         // Send high nibble
    GPIO_PORTB_DATA_R |=  (1<<4);         // RS = 1
    GPIO_PORTB_DATA_R |=  (1<<5);         // EN = 1
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);         // EN = 0

    send_To_Data_Port(data);              // Send low nibble
    GPIO_PORTB_DATA_R |=  (1<<4);         // RS = 1
    GPIO_PORTB_DATA_R |=  (1<<5);         // EN = 1
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);         // EN = 0
}

void send_LCD_Command(unsigned char command)
{
    send_To_Data_Port(command >> 4);      // Send high nibble
    GPIO_PORTB_DATA_R &= ~(1<<4);         // RS = 0
    GPIO_PORTB_DATA_R |=  (1<<5);         // EN = 1
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);         // EN = 0

    send_To_Data_Port(command);           // Send low nibble
    GPIO_PORTB_DATA_R &= ~(1<<4);         // RS = 0
    GPIO_PORTB_DATA_R |=  (1<<5);         // EN = 1
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);         // EN = 0
}

void write_LCD_String(char *str, unsigned char length)
{
    char i;
    send_LCD_Command(0x01); 
    send_LCD_Command(0x80);
    for(i = 0; i < length; i++) {
        if(i == 16) {
            send_LCD_Command(0xC0);
        }
        write_LCD_Data(str[i]);
    }
}

void initialize_LCD(void)
{
    // === Required wake-up sequence (8-bit to 4-bit mode) ===
    delay_ms(10000);
    send_To_Data_Port(0x03);
    GPIO_PORTB_DATA_R &= ~(1<<4);  // RS = 0
    GPIO_PORTB_DATA_R |= (1<<5);   // EN = 1
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);  // EN = 0

    delay_ms(10000);
    send_To_Data_Port(0x03);
    GPIO_PORTB_DATA_R |= (1<<5);
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);

    delay_ms(10000);
    send_To_Data_Port(0x03);
    GPIO_PORTB_DATA_R |= (1<<5);
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);

    delay_ms(10000);
    send_To_Data_Port(0x02);  // Switch to 4-bit mode
    GPIO_PORTB_DATA_R |= (1<<5);
    delay_ms(10000);
    GPIO_PORTB_DATA_R &= ~(1<<5);

    // === Standard LCD setup ===
    send_LCD_Command(0x28);  // Function set: 4-bit, 2 lines, 5x8 dots
    send_LCD_Command(0x06);  // Entry mode set: increment cursor
    send_LCD_Command(0x0C);  // Display on, cursor off
    send_LCD_Command(0x01);  // Clear display
}

void send_To_Data_Port(unsigned char data)
{
    data &= 0x0F;                 // Use only lower 4 bits
    GPIO_PORTB_DATA_R &= ~0x0F;  // Clear PB0-PB3
    GPIO_PORTB_DATA_R |= data;   // Set PB0-PB3 with the nibble
}

void delay_ms(long milliseconds)
{
    while(milliseconds--);
}

void set_LCD_CursorLine(uint8_t line)
{
    if (line == 1)
        send_LCD_Command(0x80);  // First line address
    else if (line == 2)
        send_LCD_Command(0xC0);  // Second line address
}
