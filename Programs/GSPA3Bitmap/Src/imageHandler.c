#include "converter.h"
#include "display.h"
#include "eCodes.h"
#include "input.h"
#include <stdbool.h>
#include "errorHandler.h"

bool isAligned = false;
int status = OK;
unsigned char trash;
unsigned char nextByte[2] = {00, 00};
uint16_t lcdColor = 0;
int absoluteSize = 0;
bool isAbsoluteMode = false;

void resetDecoderState(void) {
    status = OK;
    isAbsoluteMode = false;
    absoluteSize = 0; 
    isAligned = false;
}

void handleUncompressedImage(RGBQUAD* palette, BITMAPINFOHEADER* passport) {
  int width = (*passport).biWidth;
  int align = (((width * 8) + 31) / 32) * 4;

  for (int y = 0; y < (*passport).biHeight; y++) {
    for (int x = 0; x < (*passport).biWidth; x++) {
      COMread(nextByte, 1, 1);
      convertColor(palette[nextByte[0]], &lcdColor);
      updateAbsolutLine(lcdColor);
    }
    int dif = align - width;
    if (dif > 0) {
      for (int i = 0; i < dif; i++) {
        COMread(&trash, 1, 1);
      }
    }
    endline();
  }
}

void absoluteModeHanler(RGBQUAD* palette) {
  COMread(nextByte, 1, 1);
  absoluteSize--;
  if (absoluteSize == 0) {
    if (!isAligned) {
      COMread(&trash, 1, 1);
    }
    isAbsoluteMode = false;
  }
  convertColor(palette[nextByte[0]], &lcdColor);
  // Ausgabe
  // displayDrawAbsolut(lcdColor);
  updateAbsolutLine(lcdColor);
}

void activateAbsolut(void) {
  absoluteSize = nextByte[1];
  if (absoluteSize % 2 != 0) {
    isAligned = false;
  } else {
    isAligned = true;
  }
  isAbsoluteMode = true;
}

void statusCheck() {
  switch (status) {
  case STATUS_EOL:
    endline();
    break;

  case STATUS_END_OF_FILE:
    endline();
    // nextFileReady = true;
    break;

  case STATUS_ABSOLUTE:
    activateAbsolut();
    break;

  default:
    printError(status, __FILE__, __LINE__, MSG_ERR_UNKNOWN, false);
    break;
  }
}

void handleCompressedImage(RGBQUAD* palette) {
  COMread(nextByte, 1, 2);
  if (status == STATUS_DELTA) {
    updateDelta(nextByte[0], nextByte[1]);
    status = OK;
  } else {
    status = inputRecognizer(nextByte);

    if (status == STATUS_RLE) {
      convertColor(palette[nextByte[1]], &lcdColor);

      // Ausgabe
      // displayDrawRLE(lcdColor, nextByte[0]);
      updateRLELine(lcdColor, nextByte[0]);
    } else {
      statusCheck();
    }
  }
}

int imageHandler(BITMAPINFOHEADER* passport, RGBQUAD* palette) {
  if ((*passport).biCompression == BI_RGB) {
    handleUncompressedImage(palette, passport);
    
    status = STATUS_END_OF_FILE;

  } else if ((*passport).biCompression == BI_RLE8) {
    if (isAbsoluteMode) {
      absoluteModeHanler(palette);
    } else {
      if (status == STATUS_END_OF_FILE) {
        endline();
        return status;
      }

      else {
        handleCompressedImage(palette);
      }
    }
  }
  return status;
}
