#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "BMP_types.h"


int shakeHandsWithFile(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* passport, RGBQUAD* palette);


int skipTrash(uint32_t offset);
int readPalette(RGBQUAD *palette, uint32_t colorsSize);

#endif