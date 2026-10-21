#ifndef SOR_FRAME_PROFILE_H
#define SOR_FRAME_PROFILE_H
#include <stdint.h>
void prof_init(void);
void prof_begin(uint32_t frame,int turbo);
void prof_phase(unsigned phase);
void prof_mark(unsigned mark);
void prof_end(void);
#endif
