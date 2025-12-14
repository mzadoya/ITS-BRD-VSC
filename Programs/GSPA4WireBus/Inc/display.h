#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
void initDisplay();
void drawInfo(char* sensorName, uint64_t ID, int sensorNumber);
void drawTemperature(float temp, int sensorNumber);
#endif