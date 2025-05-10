#include <stdint.h>
#include <stdbool.h>
#ifndef AUDIO_H
#define AUDIO_H
#define LANG_ARABIC  0
#define LANG_ENGLISH 1

extern bool mute_flag;
extern uint8_t last_played_track;
extern  uint8_t language_flag;

void Audio_PlayTrack(uint8_t trackNum);
void mute(void);
void unmute(void);
void set_language_arabic(void);
void set_language_english(void);

#endif
