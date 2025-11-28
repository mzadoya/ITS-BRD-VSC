#ifndef DISPLAY_H
#define DISPLAY_H

#include "LCD_general.h"
#include <stdint.h>
void initCoords(int32_t width, int32_t height);

int updateCoords(int32_t width, int32_t height, Coordinate* coords);

int inputRecognizer(char* info);
int displayDrawRLE(uint16_t lcdColor, int size);
int displayDrawAbsolut(uint16_t lcdColor);
int updateDelta(uint32_t x, uint32_t y);
int endline();
int updateRLELine(uint16_t lcdColor, int size);
int updateAbsolutLine(uint16_t lcdColor);
int lcdPrintLine();
#endif