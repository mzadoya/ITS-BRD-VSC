#ifndef ROTARY_DECODER_H
#define ROTARY_DECODER_H

#include <stdint.h>
extern uint32_t transitionCount;
void initDecoder(void);
int ecoderUpdate(uint8_t eingabe, uint8_t *direction);
void updateSpeed(double *speed);
void updateAngle(double *wAngle);
extern double speed ;
extern double angle;
#endif