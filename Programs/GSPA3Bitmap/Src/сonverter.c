

#include "BMP_types.h"


int convertColor(RGBQUAD palette, uint16_t* lcdColor) {
    uint16_t red = palette.rgbRed >> 3; //zB 1111 1010 -> 0001 1111
    uint16_t green = palette.rgbGreen >> 2; //zB 1011 0100 -> 0010 1101
    uint16_t blue = palette.rgbBlue >> 3; //zB 0001 1110 - > 0000 0011

    *lcdColor = (blue | green << 5 | red << 11); // 1111 1101 1010 0011
    
    return 0;
}