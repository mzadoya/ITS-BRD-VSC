#ifndef LED_H
#define LED_H

#include <stdint.h>
void initLed();

void ledSetError();

void ledSetDirection(uint8_t direction);

void ledClear();

#endif