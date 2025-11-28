#ifndef IMAGE_HANDLER_H
#define IMAGE_HANDLER_H
#include "BMP_types.h"
#include <stdbool.h>
int imageHandler(BITMAPINFOHEADER* passport, RGBQUAD* palette);
void resetDecoderState(void);
#endif