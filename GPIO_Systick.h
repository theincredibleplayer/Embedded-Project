#include <stdint.h>

#ifndef GPIO_Systick
#define GPIO_Systick

void SysTick_Init();
void SysTick_Wait(uint32_t);
void SysTick_Wait1ms(uint32_t);
void PORTB_Init(void);



#endif