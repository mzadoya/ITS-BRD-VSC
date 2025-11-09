#ifndef ROTARY_DECODER_H
#define ROTARY_DECODER_H

#include <stdint.h>
void timerStart(void);
void updateTime(uint32_t *timeD);
int ecoderUpdate(uint8_t eingabe, uint8_t *direction);
void updateSpeed(double *speed);

#endif