#include <stdint.h>

#ifndef UART_CONFIG
#define UART_CONFIG

void UART0_Init(void);
void UART0_SendChar(char c);
void UART0_SendString(const char *str);
void UART0_SendNumberInt(int num);
void UART0_SendNumberFloat(float num,uint8_t precision);
void UART0_SendNewLine();

void UART5_Init(void);
char UART5_ReceiveChar(void);
char *UART5_ReceiveString(char *string);


#endif