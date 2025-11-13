#ifndef ROTARY_DECODER_H
#define ROTARY_DECODER_H

#include <stdint.h>
extern int32_t transitionCount;
void initDecoder(void);
int ecoderUpdate(uint16_t eingabe, uint16_t *direction, uint32_t time2);
void updateSpeed(double *speed);
void updateAngle(double *wAngle);
extern double speed ;
extern double angle;

#endif