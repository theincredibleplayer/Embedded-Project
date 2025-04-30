#ifndef UART_CONFIG
#define UART_CONFIG

void UART0_Init(void);
void UART0_SendChar(char c);
void UART0_SendString(const char *str);
void UART0_SendNumber(int num);

void UART5_Init(void);
char UART5_ReceiveChar(void);
char *UART5_ReceiveString(char *string);


void PORTB_Init(void);

#endif // UTILITIES_H