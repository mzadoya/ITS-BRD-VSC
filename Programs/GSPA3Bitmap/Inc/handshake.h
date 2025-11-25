#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "BMP_types.h"


int shakeHandsWithFile(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* passport, RGBQUAD* palette);

#endif