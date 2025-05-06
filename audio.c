#include "audio.h"
#include "uart_c.h"

void Audio_PlayTrack(uint8_t trackNum) {
    int i;
    uint8_t command[8];

    command[0] = 0x7E;
    command[1] = 0xFF;
    command[2] = 0x06;
    command[3] = 0x03;
    command[4] = 0x00;
    command[5] = 0x00;
    command[6] = trackNum;
    command[7] = 0xEF;

    for(i = 0; i < 8; i++) {
        UART3_Send(command[i]);
    }
}
