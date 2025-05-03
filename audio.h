#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>

void UART1_Init(void);
void UART1_Transmit(uint8_t data);
void DFPlayer_PlayTrack(uint8_t trackNum);

#endif // AUDIO_H
