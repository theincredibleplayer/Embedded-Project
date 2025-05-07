#include <stdint.h>
#include <stdbool.h>
#ifndef AUDIO_H
#define AUDIO_H

extern bool mute_flag;

void Audio_PlayTrack(uint8_t trackNum);
void mute(void);
void unmute(void);

#endif
