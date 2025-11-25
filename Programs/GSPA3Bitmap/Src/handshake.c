
#include "handshake.h"
#include "eCodes.h"
#include "BMP_types.h"
#include "MS_basetypes.h"
#include <stdint.h>
#include <stdio.h>
#include "input.h"


#define BITS_PER_PIXEL 8
#define POS_AFTER_HANDSHAKE (14 + 40 + 1024)
#define CHUNK 64

int readHeader(BITMAPFILEHEADER *fileHeader) {
    char buffer[4];
    COMread(buffer, 1, 2);
    fileHeader->bfType = (unsigned int)buffer[0] | (unsigned int)buffer[1] << 8;

    if (fileHeader->bfType != BMP_SIGNATURE) {
        return ERR_NOT_BMP;
    }
    COMread(buffer, 1, 4);
    fileHeader->bfSize = (unsigned int)buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int)buffer[2] << 16 | (unsigned int) buffer[3] << 24;

    COMread(buffer, 1, 2);
    fileHeader->bfReserved1 = (unsigned int)buffer[0] | (unsigned int)buffer[1] << 8;

    COMread(buffer, 1, 2);
    fileHeader->bfReserved2 = (unsigned int)buffer[0] | (unsigned int)buffer[1] << 8;

    COMread(buffer, 1, 4);
    fileHeader->bfOffBits = (unsigned int)buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int)buffer[2] << 16 | (unsigned int) buffer[3] << 24;

    return OK;
}

int readPassport(BITMAPINFOHEADER *passport) {
    char buffer[4];

    COMread(buffer, 1, 4);
    passport->biSize = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int) buffer[2] << 16 | (unsigned int) buffer[3] << 24;

    COMread(buffer, 1, 4);
    passport->biWidth = (int) buffer[0] | (int) buffer[1] << 8 | (int) buffer[2] << 16 | (int) buffer[3] << 24;

    COMread(buffer, 1, 4);
    passport->biHeight = (int) buffer[0] | (int) buffer[1] << 8 | (int) buffer[2] << 16 | (int) buffer[3] << 24;

    COMread(buffer, 1, 2);
    passport->biPlanes = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8;

    COMread(buffer, 1, 2);
    passport->biBitCount = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8;

    COMread(buffer, 1, 4);
    passport->biCompression = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int) buffer[2] << 16 | (unsigned int) buffer[3] << 24;

     if (passport->biBitCount != BITS_PER_PIXEL || passport->biCompression != BI_RLE8) {
        return ERR_WRONG_FORMAT;
    }

    COMread(buffer, 1, 4);
    passport->biSizeImage = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int) buffer[2] << 16 | (unsigned int) buffer[3] << 24;

    COMread(buffer, 1, 4);
    passport->biXPelsPerMeter = (int) buffer[0] | (int) buffer[1] << 8 | (int) buffer[2] << 16 | (int) buffer[3] << 24;

    COMread(buffer, 1, 4);
    passport->biYPelsPerMeter = (int) buffer[0] | (int) buffer[1] << 8 | (int) buffer[2] << 16 | (int) buffer[3] << 24;

    COMread(buffer, 1, 4);
    passport->biClrUsed = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int) buffer[2] << 16 | (unsigned int) buffer[3] << 24;

    COMread(buffer, 1, 4);
    passport->biClrImportant = (unsigned int) buffer[0] | (unsigned int) buffer[1] << 8 | (unsigned int) buffer[2] << 16 | (unsigned int) buffer[3] << 24;

    // COMread(&passport, sizeof(passport), 1);
    
    return OK;
}

int readPalette(RGBQUAD *palette) {
    char buffer[4] = {0, 0, 0, 0};
    for (int i = 0; i < MAX_COLOR_TABLE_SIZE; i++) {
        COMread(buffer, 1, 4);
        (*palette).rgbRed = (unsigned char) buffer[2];
        (*palette).rgbGreen = (unsigned char) buffer[1];
        (*palette).rgbBlue = (unsigned char) buffer[0];
        (*palette).rgbReserved = (unsigned char) buffer[3];
        palette++;
    }
    return OK;
}

int skipTrash(uint32_t offset) {
    if (POS_AFTER_HANDSHAKE >= offset) {
        return ERR_READ_FAIL;
    }
    uint32_t bytesToSkip = offset - POS_AFTER_HANDSHAKE;
    char trash[CHUNK];
    while (bytesToSkip > 0) {
        if (bytesToSkip > CHUNK) {
             COMread(trash, 1, CHUNK);
             bytesToSkip = bytesToSkip - CHUNK;
        }
        else {
            COMread(trash, 1, bytesToSkip);
            bytesToSkip = 0;
        }
    }
   return OK;
}

int shakeHandsWithFile(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* passport, RGBQUAD* palette) {
   int rcShake = readHeader(fileHeader);
   if (rcShake != OK) {
    return rcShake;
   }
   int rcPassport = readPassport(passport);
   if (rcPassport != OK) {
    return rcPassport;
   }
   int rcPalette = readPalette(palette);
   if (rcPalette != OK) {
    return rcPalette;
   }
   if (POS_AFTER_HANDSHAKE != (*fileHeader).bfOffBits) {
        int rcSkip = skipTrash((*fileHeader).bfOffBits);
        return rcSkip;
   }
   return rcPalette;
}

