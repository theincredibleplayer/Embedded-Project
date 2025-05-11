#include "audio.h"
#include "uart_c.h"

bool mute_flag = false;
uint8_t last_played_track = 255; // no track has been playsed
uint8_t language_flag = LANG_ARABIC;		// Default



void mute(void) {
    mute_flag = true;
}

void unmute(void) {
    mute_flag = false;
}

void set_language_arabic(void) {
    language_flag = LANG_ARABIC;
}

void set_language_english(void) {
    language_flag = LANG_ENGLISH;
}


void Audio_PlayTrack(uint8_t trackNum) {
    int i;
    uint8_t command[8];
		if (mute_flag) return;
		if (language_flag == LANG_ENGLISH) {
			trackNum += 5;
    }
		if (trackNum == last_played_track) return;
		
		last_played_track = trackNum;
	

		
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
