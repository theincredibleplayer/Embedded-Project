#include <stdint.h>

#ifndef LCD
#define LCD

void delay_ms(long milliseconds);

void send_To_Data_Port(unsigned char data); //Maps each bit of the data byte to the correct Port B pin 

void write_LCD_Data(unsigned char data);   //Sends data (a character) to be displayed on the LCD.

void send_LCD_Command(unsigned char command);  //Sends a command to the LCD (clear display, set cursor position).

void write_LCD_String(char *str, unsigned char length); //Writes a string of given length to the LCD.

void write_LCD_Line2_NoClear(char *str, unsigned char length); //Writes a string of given length to the second line of the LCD without clearing.

void initialize_LCD(void); //Sends standard initialization commands to configure the LCD display.

void initialiaze_LCD_Ports(void);

void set_LCD_CursorLine(uint8_t line);

#endif 