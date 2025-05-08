#ifndef BLUETOOTH_MODULE
#define BLUETOOTH_MODULE

void Bluetooth(void);
void UART7_Init(void); // Initialize UART2 module for HC-05
void Delay(unsigned long counter);
char Bluetooth_Read(void); //Read data from Rx2 pin of TM4C123
void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx2 pin 
void Bluetooth_Write_String(char *str); // Transmit a string to HC-05 over Tx2 pin 

#endif