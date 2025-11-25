#ifndef DISPLAY_H
#define DISPLAY_H

#include "LCD_general.h"
#include <stdint.h>
void initCoords(int32_t width, int32_t height);

int updateCoords(int32_t width, int32_t height, Coordinate* coords);

int inputRecognizer(char* info);
int displayDraw(uint16_t lcdColor);
#endif