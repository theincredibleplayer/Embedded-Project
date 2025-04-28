#ifndef UART_CONFIG
#define UART_CONFIG

void UART0_Init(void);
void UART0_SendChar(char c);

void UART5_Init(void);
char UART5_ReceiveChar(void);

void PORTB_Init(void);

#endif // UTILITIES_H